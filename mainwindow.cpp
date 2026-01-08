/*
 * mainwindow.cpp - Main Window Implementation
 *
 * Purpose: Implements the main application window functionality
 */

#include "mainwindow.h"
#include "videoprocessor.h"
#include "colorselectdialog.h"
#include "ocrresultdialog.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , camera(nullptr)
    , captureSession(nullptr)
    , videoSink(nullptr)
    , videoProcessor(nullptr)
    , colorDialog(nullptr)
    , ocrDialog(nullptr)
    , isCameraActive(false)
    , currentColorSchemeIndex(0)
{
    // Initialize color schemes as specified in requirements
    colorSchemes.append({"White on Black", QColor("#ffffff"), QColor("#000000")});
    colorSchemes.append({"Black on White", QColor("#000000"), QColor("#ffffff")});
    colorSchemes.append({"Green on Black", QColor("#11c70e"), QColor("#000000")});
    colorSchemes.append({"Yellow on Black", QColor("#f4d81e"), QColor("#000000")});

    // Set up the user interface
    setupUI();

    // Initialize camera and video capture
    setupCamera();

    // Create video processor for frame processing and OCR
    videoProcessor = new VideoProcessor(this);

    // Connect signal from video processor when OCR completes
    connect(videoProcessor, &VideoProcessor::ocrComplete,
            this, &MainWindow::onOCRComplete);
}

MainWindow::~MainWindow()
{
    // Stop camera before destruction
    if (camera) {
        camera->stop();
    }

    // Qt's parent-child relationship will automatically delete child objects
}

void MainWindow::setupUI()
{
    // Set window properties
    setWindowTitle("Video OCR Application");
    resize(800, 600);

    // Create central widget and main layout
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);

    // Create video widget for displaying camera feed
    videoWidget = new QVideoWidget(this);
    videoWidget->setMinimumSize(640, 480);
    mainLayout->addWidget(videoWidget);

    // Create control panel layout (horizontal)
    QHBoxLayout *controlLayout = new QHBoxLayout();

    // Start/Stop button
    startStopButton = new QPushButton("Start Camera", this);
    connect(startStopButton, &QPushButton::clicked,
            this, &MainWindow::onStartStopClicked);
    controlLayout->addWidget(startStopButton);

    // Color scheme selector
    QLabel *colorLabel = new QLabel("Color Scheme:", this);
    controlLayout->addWidget(colorLabel);

    colorSchemeCombo = new QComboBox(this);
    for (const auto &scheme : colorSchemes) {
        colorSchemeCombo->addItem(scheme.name);
    }
    connect(colorSchemeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onColorSchemeChanged);
    controlLayout->addWidget(colorSchemeCombo);

    controlLayout->addStretch();  // Push controls to the left

    mainLayout->addLayout(controlLayout);

    // Status label for displaying information
    statusLabel = new QLabel("Press F4 to capture and perform OCR", this);
    statusLabel->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 5px; }");
    mainLayout->addWidget(statusLabel);

    // Set the central widget
    setCentralWidget(centralWidget);
}

void MainWindow::setupCamera()
{
    // Create camera object (default camera)
    camera = new QCamera(this);

    // Create video sink to receive video frames
    videoSink = new QVideoSink(this);

    // Connect video sink's videoFrameChanged signal to our slot
    // This is called every time a new video frame is available
    connect(videoSink, &QVideoSink::videoFrameChanged,
            this, &MainWindow::onVideoFrameChanged);

    // Create capture session to manage camera and outputs
    captureSession = new QMediaCaptureSession(this);
    captureSession->setCamera(camera);
    captureSession->setVideoOutput(videoWidget);  // Display on video widget
    captureSession->setVideoSink(videoSink);       // Also send to our sink for processing

    // Check for camera errors
    connect(camera, &QCamera::errorOccurred, this, [this](QCamera::Error error, const QString &errorString) {
        QMessageBox::critical(this, "Camera Error",
                              QString("Camera error: %1").arg(errorString));
        statusLabel->setText("Camera error occurred");
    });
}

void MainWindow::onStartStopClicked()
{
    if (!isCameraActive) {
        // Start the camera
        camera->start();
        isCameraActive = true;
        startStopButton->setText("Stop Camera");
        statusLabel->setText("Camera active - Press F4 to capture and perform OCR");
    } else {
        // Stop the camera
        camera->stop();
        isCameraActive = false;
        startStopButton->setText("Start Camera");
        statusLabel->setText("Camera stopped");
    }
}

void MainWindow::onColorSchemeChanged(int index)
{
    // Update current color scheme index
    currentColorSchemeIndex = index;

    // If video processor exists, update its color scheme
    if (videoProcessor) {
        const auto &scheme = colorSchemes[index];
        videoProcessor->setColorScheme(scheme.foreground, scheme.background);
    }

    statusLabel->setText(QString("Color scheme changed to: %1").arg(colorSchemes[index].name));
}

void MainWindow::onVideoFrameChanged(const QVideoFrame &frame)
{
    // Store the current frame for OCR capture
    currentFrame = frame;

    // Process the frame if video processor is ready
    if (videoProcessor) {
        // Create a copy since QVideoFrame may be modified
        QVideoFrame frameCopy = frame;
        videoProcessor->processFrame(frameCopy,
                                     colorSchemes[currentColorSchemeIndex].foreground,
                                     colorSchemes[currentColorSchemeIndex].background);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Check if F4 key was pressed
    if (event->key() == Qt::Key_F4) {
        if (isCameraActive) {
            captureAndPerformOCR();
        } else {
            statusLabel->setText("Please start the camera first");
        }
        event->accept();  // Mark event as handled
    } else {
        // Pass other keys to base class
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::captureAndPerformOCR()
{
    // Check if we have a valid frame
    if (!currentFrame.isValid()) {
        statusLabel->setText("No frame available for OCR");
        return;
    }

    statusLabel->setText("Performing OCR...");

    // Create a copy of the frame for OCR processing
    QVideoFrame frameCopy = currentFrame;

    // Process OCR in video processor (runs in separate thread)
    videoProcessor->performOCR(frameCopy,
                               colorSchemes[currentColorSchemeIndex].foreground,
                               colorSchemes[currentColorSchemeIndex].background);
}

void MainWindow::onOCRComplete(const QString &text)
{
    // Update status
    statusLabel->setText("OCR complete");

    // Create or update OCR result dialog
    if (!ocrDialog) {
        ocrDialog = new OCRResultDialog(this);
    }

    // Display the recognized text
    ocrDialog->setOCRText(text);
    ocrDialog->show();
    ocrDialog->raise();      // Bring to front
    ocrDialog->activateWindow();  // Give focus
}
