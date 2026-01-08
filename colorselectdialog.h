/*
 * colorselectdialog.h - Color Selection Dialog Header
 *
 * Purpose: Dialog window for selecting color schemes
 * Displays available color options and allows user selection
 */

#ifndef COLORSELECTDIALOG_H
#define COLORSELECTDIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QColor>

class ColorSelectDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit ColorSelectDialog(QWidget *parent = nullptr);

    // Get the selected color scheme index
    int getSelectedSchemeIndex() const;

    // Set the default selected scheme
    void setDefaultScheme(int index);

private slots:
    // Slot: Called when OK button is clicked
    void onOKClicked();

    // Slot: Called when Cancel button is clicked
    void onCancelClicked();

private:
    // Setup the user interface
    void setupUI();

    // UI Components
    QButtonGroup *colorGroup;          // Button group for radio buttons
    QRadioButton *whiteOnBlack;        // White text on black background
    QRadioButton *blackOnWhite;        // Black text on white background
    QRadioButton *greenOnBlack;        // Green text on black background
    QRadioButton *yellowOnBlack;       // Yellow text on black background

    QPushButton *okButton;             // OK button
    QPushButton *cancelButton;         // Cancel button

    int selectedIndex;                 // Currently selected index
};

#endif // COLORSELECTDIALOG_H
