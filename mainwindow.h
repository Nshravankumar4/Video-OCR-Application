/*
 * mainwindow.h - Main Window Header
 *
 * Purpose: Defines the main application window that contains:
 * - Video display area
 * - Camera controls (start/stop)
 * - Color scheme selection
 * - Capture functionality (F4 key)
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QVideoSink>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

// Forward declarations to avoid circular dependencies
class VideoProcessor;
class ColorSelectDialog;
class OCRResultDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT  // Required macro for Qt's meta-object system (signals/slots)

public:
    // Constructor: Initialize the main window
    explicit MainWindow(QWidget *parent = nullptr);

    // Destructor: Clean up resources
    ~MainWindow();

protected:
    // Override keyPressEvent to capture F4 key for OCR
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // Slot: Called when Start/Stop button is clicked
    void onStartStopClicked();

    // Slot: Called when color scheme is changed
    void onColorSchemeChanged(int index);

    // Slot: Called when a new video frame is available
    void onVideoFrameChanged(const QVideoFrame &frame);

    // Slot: Called when OCR processing is complete
    void onOCRComplete(const QString &text);

private:
    // Private method: Set up the user interface
    void setupUI();

    // Private method: Initialize camera and capture session
    void setupCamera();

    // Private method: Perform OCR on current frame
    void captureAndPerformOCR();

    // UI Components
    QWidget *centralWidget;           // Central widget container
    QVBoxLayout *mainLayout;          // Main vertical layout
    QVideoWidget *videoWidget;        // Widget to display video
    QPushButton *startStopButton;     // Button to start/stop camera
    QComboBox *colorSchemeCombo;      // Dropdown for color schemes
    QLabel *statusLabel;              // Status information display

    // Camera Components
    QCamera *camera;                  // Camera object for video capture
    QMediaCaptureSession *captureSession;  // Session managing camera
    QVideoSink *videoSink;           // Sink to receive video frames

    // Processing Components
    VideoProcessor *videoProcessor;   // Handles frame processing and OCR

    // Dialog Windows
    ColorSelectDialog *colorDialog;   // Dialog for color selection
    OCRResultDialog *ocrDialog;       // Dialog to display OCR results

    // State Variables
    bool isCameraActive;              // Track camera state
    QVideoFrame currentFrame;         // Store latest video frame for OCR

    // Color Schemes (foreground, background)
    struct ColorScheme {
        QString name;
        QColor foreground;
        QColor background;
    };
    QList<ColorScheme> colorSchemes;  // List of available color schemes
    int currentColorSchemeIndex;      // Currently selected scheme
};

#endif // MAINWINDOW_H
