/*
 * ocrresultdialog.cpp - OCR Result Dialog Implementation
 *
 * Purpose: Implements the OCR result display dialog
 */

#include "ocrresultdialog.h"
#include <QApplication>
#include <QHBoxLayout>

OCRResultDialog::OCRResultDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
}

void OCRResultDialog::setupUI()
{
    // Set dialog properties
    setWindowTitle("OCR Results");
    resize(600, 400);

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Add title label
    QLabel *titleLabel = new QLabel("Recognized Text:", this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; }");
    mainLayout->addWidget(titleLabel);

    // Create text edit for displaying OCR results
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(false);  // Allow user to edit if needed
    textEdit->setPlaceholderText("OCR results will appear here...");

    // Set font for better readability
    QFont font("Courier New", 10);
    textEdit->setFont(font);

    mainLayout->addWidget(textEdit);

    // Status label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("QLabel { color: gray; font-size: 10px; }");
    mainLayout->addWidget(statusLabel);

    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Copy button - copies text to clipboard
    copyButton = new QPushButton("Copy to Clipboard", this);
    copyButton->setToolTip("Copy the recognized text to clipboard");
    connect(copyButton, &QPushButton::clicked, this, &OCRResultDialog::onCopyClicked);
    buttonLayout->addWidget(copyButton);

    // Clear button - clears the text
    clearButton = new QPushButton("Clear", this);
    clearButton->setToolTip("Clear the text display");
    connect(clearButton, &QPushButton::clicked, this, &OCRResultDialog::onClearClicked);
    buttonLayout->addWidget(clearButton);

    // Add stretch to push close button to the right
    buttonLayout->addStretch();

    // Close button
    closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &OCRResultDialog::onCloseClicked);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void OCRResultDialog::setOCRText(const QString &text)
{
    // Set the text in the text edit
    textEdit->setPlainText(text);

    // Update status label with character count
    int charCount = text.length();
    int wordCount = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).count();

    statusLabel->setText(QString("Characters: %1 | Words: %2")
                             .arg(charCount)
                             .arg(wordCount));
}

QString OCRResultDialog::getOCRText() const
{
    return textEdit->toPlainText();
}

void OCRResultDialog::onCopyClicked()
{
    // Get the text from text edit
    QString text = textEdit->toPlainText();

    if (text.isEmpty()) {
        statusLabel->setText("No text to copy");
        return;
    }

    // Copy to clipboard
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);

    // Update status
    statusLabel->setText("Text copied to clipboard!");

    // Create a timer to clear the status message after 3 seconds
    QTimer::singleShot(3000, this, [this]() {
        int charCount = textEdit->toPlainText().length();
        int wordCount = textEdit->toPlainText().split(QRegularExpression("\\s+"),
                                                      Qt::SkipEmptyParts).count();
        statusLabel->setText(QString("Characters: %1 | Words: %2")
                                 .arg(charCount)
                                 .arg(wordCount));
    });
}

void OCRResultDialog::onClearClicked()
{
    // Clear the text edit
    textEdit->clear();
    statusLabel->setText("Text cleared");
}

void OCRResultDialog::onCloseClicked()
{
    // Close the dialog
    close();
}
