/*
 * videoprocessor.cpp - Video Frame Processor Implementation
 *
 * Purpose: Implements video frame processing and OCR functionality
 */

#include "videoprocessor.h"
#include <QDebug>
#include <QImage>

// OCRWorker Implementation
OCRWorker::OCRWorker(QObject *parent)
    : QObject(parent)
    , tessApi(nullptr)
{
    // Initialize Tesseract API
    tessApi = new tesseract::TessBaseAPI();

    // Initialize with English language
    // Make sure tessdata folder exists in application directory
    // or set TESSDATA_PREFIX environment variable
    if (tessApi->Init(nullptr, "eng")) {
        qWarning() << "Could not initialize Tesseract API";
        qWarning() << "Make sure tessdata folder is in the application directory";
        delete tessApi;
        tessApi = nullptr;
    } else {
        // Set page segmentation mode for better OCR results
        // PSM_AUTO: Fully automatic page segmentation
        tessApi->SetPageSegMode(tesseract::PSM_AUTO);
    }
}

OCRWorker::~OCRWorker()
{
    // Clean up Tesseract API
    if (tessApi) {
        tessApi->End();
        delete tessApi;
    }
}

void OCRWorker::processOCR(const cv::Mat &image)
{
    QString result;

    // Check if Tesseract is initialized
    if (!tessApi) {
        result = "Error: Tesseract not initialized";
        emit ocrComplete(result);
        return;
    }

    // Check if image is valid
    if (image.empty()) {
        result = "Error: Invalid image";
        emit ocrComplete(result);
        return;
    }

    try {
        // Set the image for OCR processing
        // Tesseract expects grayscale or color image
        tessApi->SetImage(image.data, image.cols, image.rows,
                          image.channels(), image.step);

        // Perform OCR and get text
        char* outText = tessApi->GetUTF8Text();

        if (outText) {
            result = QString::fromUtf8(outText);
            delete[] outText;  // Free memory allocated by Tesseract
        } else {
            result = "No text recognized";
        }

    } catch (const std::exception &e) {
        result = QString("OCR Error: %1").arg(e.what());
        qWarning() << result;
    }

    // Emit the result
    emit ocrComplete(result);
}

// VideoProcessor Implementation
VideoProcessor::VideoProcessor(QObject *parent)
    : QObject(parent)
    , ocrThread(nullptr)
    , ocrWorker(nullptr)
    , foregroundColor(Qt::white)
    , backgroundColor(Qt::black)
{
    // Create OCR worker and thread
    ocrThread = new QThread(this);
    ocrWorker = new OCRWorker();

    // Move worker to separate thread to avoid blocking UI
    ocrWorker->moveToThread(ocrThread);

    // Connect signals and slots
    connect(this, &VideoProcessor::requestOCR,
            ocrWorker, &OCRWorker::processOCR);

    connect(ocrWorker, &OCRWorker::ocrComplete,
            this, &VideoProcessor::ocrComplete);

    // Clean up worker when thread finishes
    connect(ocrThread, &QThread::finished,
            ocrWorker, &OCRWorker::deleteLater);

    // Start the OCR thread
    ocrThread->start();
}

VideoProcessor::~VideoProcessor()
{
    // Stop and wait for OCR thread to finish
    if (ocrThread) {
        ocrThread->quit();
        ocrThread->wait();
    }
}

void VideoProcessor::setColorScheme(const QColor &fgColor, const QColor &bgColor)
{
    foregroundColor = fgColor;
    backgroundColor = bgColor;
}

cv::Mat VideoProcessor::videoFrameToMat(QVideoFrame &frame)
{
    // Ensure frame is mapped to memory for reading
    if (!frame.map(QVideoFrame::ReadOnly)) {
        qWarning() << "Failed to map video frame";
        return cv::Mat();
    }

    // Get frame properties
    QSize frameSize = frame.size();
    int width = frameSize.width();
    int height = frameSize.height();

    // Convert QVideoFrame to QImage first
    QImage image = frame.toImage();

    // Unmap the frame
    frame.unmap();

    // Check if conversion was successful
    if (image.isNull()) {
        qWarning() << "Failed to convert video frame to image";
        return cv::Mat();
    }

    // Convert QImage to OpenCV Mat
    // QImage format: RGB32 or ARGB32 (4 bytes per pixel)
    image = image.convertToFormat(QImage::Format_RGB888);

    // Create OpenCV Mat from QImage data
    cv::Mat mat(image.height(), image.width(), CV_8UC3,
                const_cast<uchar*>(image.bits()), image.bytesPerLine());

    // Clone the Mat to ensure data persistence
    // (QImage data may be deleted when image goes out of scope)
    return mat.clone();
}

cv::Mat VideoProcessor::convertToMonochrome(const cv::Mat &input,
                                            const QColor &fgColor,
                                            const QColor &bgColor)
{
    // Check if input is valid
    if (input.empty()) {
        return cv::Mat();
    }

    // Convert to grayscale first
    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else if (input.channels() == 4) {
        cv::cvtColor(input, gray, cv::COLOR_BGRA2GRAY);
    } else {
        gray = input.clone();
    }

    // Apply binary threshold to create monochrome image
    // This converts grayscale to pure black and white
    cv::Mat binary;
    cv::threshold(gray, binary, 128, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // Create color monochrome image with custom colors
    cv::Mat colorMono(binary.size(), CV_8UC3);

    // Get color values
    cv::Vec3b fg(fgColor.blue(), fgColor.green(), fgColor.red());  // BGR order
    cv::Vec3b bg(bgColor.blue(), bgColor.green(), bgColor.red());

    // Apply custom colors
    for (int y = 0; y < binary.rows; y++) {
        for (int x = 0; x < binary.cols; x++) {
            // If pixel is white in binary image, use foreground color
            // Otherwise use background color
            if (binary.at<uchar>(y, x) > 128) {
                colorMono.at<cv::Vec3b>(y, x) = fg;
            } else {
                colorMono.at<cv::Vec3b>(y, x) = bg;
            }
        }
    }

    return colorMono;
}

void VideoProcessor::processFrame(QVideoFrame &frame,
                                  const QColor &fgColor,
                                  const QColor &bgColor)
{
    // Convert video frame to OpenCV Mat
    cv::Mat mat = videoFrameToMat(frame);

    if (mat.empty()) {
        return;
    }

    // Convert to monochrome with specified colors
    cv::Mat monochrome = convertToMonochrome(mat, fgColor, bgColor);

    // Note: The processed frame is not displayed back to the video widget
    // in this implementation. If you want to display the processed frame,
    // you would need to convert the Mat back to QVideoFrame and emit it
    // through a custom video sink. This is left as an exercise or can be
    // implemented if needed.
}

void VideoProcessor::performOCR(QVideoFrame &frame,
                                const QColor &fgColor,
                                const QColor &bgColor)
{
    // Convert video frame to OpenCV Mat
    cv::Mat mat = videoFrameToMat(frame);

    if (mat.empty()) {
        emit ocrComplete("Error: Could not process frame");
        return;
    }

    // Convert to monochrome for better OCR results
    cv::Mat monochrome = convertToMonochrome(mat, fgColor, bgColor);

    if (monochrome.empty()) {
        emit ocrComplete("Error: Could not convert to monochrome");
        return;
    }

    // Convert to grayscale for Tesseract (it works better with grayscale)
    cv::Mat gray;
    cv::cvtColor(monochrome, gray, cv::COLOR_BGR2GRAY);

    // Request OCR processing in worker thread
    emit requestOCR(gray);
}
