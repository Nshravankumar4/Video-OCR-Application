/*
 * ocrresultdialog.h - OCR Result Dialog Header
 *
 * Purpose: Dialog window for displaying OCR recognition results
 * Shows the extracted text from the captured video frame
 */

#ifndef OCRRESULTDIALOG_H
#define OCRRESULTDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QClipboard>

class OCRResultDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit OCRResultDialog(QWidget *parent = nullptr);

    // Set the OCR result text to display
    void setOCRText(const QString &text);

    // Get the current displayed text
    QString getOCRText() const;

private slots:
    // Slot: Copy text to clipboard
    void onCopyClicked();

    // Slot: Clear the text
    void onClearClicked();

    // Slot: Close the dialog
    void onCloseClicked();

private:
    // Setup the user interface
    void setupUI();

    // UI Components
    QTextEdit *textEdit;              // Text display area
    QPushButton *copyButton;          // Copy to clipboard button
    QPushButton *clearButton;         // Clear text button
    QPushButton *closeButton;         // Close dialog button
    QLabel *statusLabel;              // Status information
};

#endif // OCRRESULTDIALOG_H
