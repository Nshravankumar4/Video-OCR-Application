/*
 * videoprocessor.h - Video Frame Processor Header
 *
 * Purpose: Handles video frame processing including:
 * - Conversion from QVideoFrame to OpenCV Mat
 * - Monochrome conversion with custom color schemes
 * - OCR processing using Tesseract
 */

#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QVideoFrame>
#include <QImage>
#include <QColor>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

// Worker class for OCR processing in separate thread
// This prevents UI freezing during OCR operations
class OCRWorker : public QObject
{
    Q_OBJECT

public:
    explicit OCRWorker(QObject *parent = nullptr);
    ~OCRWorker();

public slots:
    // Slot: Process OCR on the given image
    void processOCR(const cv::Mat &image);

signals:
    // Signal: Emitted when OCR processing is complete
    void ocrComplete(const QString &text);

private:
    tesseract::TessBaseAPI *tessApi;  // Tesseract OCR API instance
};

// Main video processor class
class VideoProcessor : public QObject
{
    Q_OBJECT

public:
    explicit VideoProcessor(QObject *parent = nullptr);
    ~VideoProcessor();

    // Process a video frame: convert to monochrome and update display
    void processFrame(QVideoFrame &frame, const QColor &fgColor, const QColor &bgColor);

    // Perform OCR on a captured frame
    void performOCR(QVideoFrame &frame, const QColor &fgColor, const QColor &bgColor);

    // Set the color scheme for monochrome conversion
    void setColorScheme(const QColor &fgColor, const QColor &bgColor);

signals:
    // Signal: Emitted when OCR processing is complete
    void ocrComplete(const QString &text);

    // Signal: Request OCR processing in worker thread
    void requestOCR(const cv::Mat &image);

private:
    // Convert QVideoFrame to OpenCV Mat
    cv::Mat videoFrameToMat(QVideoFrame &frame);

    // Convert to monochrome using specified colors
    cv::Mat convertToMonochrome(const cv::Mat &input, const QColor &fgColor, const QColor &bgColor);

    // OCR worker and thread
    QThread *ocrThread;       // Separate thread for OCR
    OCRWorker *ocrWorker;     // Worker object for OCR processing

    // Current color scheme
    QColor foregroundColor;
    QColor backgroundColor;
};

#endif // VIDEOPROCESSOR_H
