/*
 * colorselectdialog.cpp - Color Selection Dialog Implementation
 *
 * Purpose: Implements the color scheme selection dialog
 */

#include "colorselectdialog.h"
#include <QHBoxLayout>

ColorSelectDialog::ColorSelectDialog(QWidget *parent)
    : QDialog(parent)
    , colorGroup(nullptr)
    , selectedIndex(0)
{
    setupUI();
}

void ColorSelectDialog::setupUI()
{
    // Set dialog properties
    setWindowTitle("Select Color Scheme");
    setModal(true);  // Make dialog modal (blocks parent window)
    resize(300, 250);

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Add title label
    QLabel *titleLabel = new QLabel("Choose a color scheme for monochrome conversion:", this);
    titleLabel->setWordWrap(true);
    mainLayout->addWidget(titleLabel);

    // Create button group for radio buttons
    // Button group ensures only one radio button can be selected at a time
    colorGroup = new QButtonGroup(this);

    // Create radio buttons for each color scheme
    // Scheme 1: White on Black (#ffffff on #000000)
    whiteOnBlack = new QRadioButton("White on Black", this);
    whiteOnBlack->setStyleSheet("QRadioButton { color: white; background-color: black; padding: 5px; }");
    colorGroup->addButton(whiteOnBlack, 0);
    mainLayout->addWidget(whiteOnBlack);

    // Scheme 2: Black on White (#000000 on #ffffff)
    blackOnWhite = new QRadioButton("Black on White", this);
    blackOnWhite->setStyleSheet("QRadioButton { color: black; background-color: white; padding: 5px; border: 1px solid black; }");
    colorGroup->addButton(blackOnWhite, 1);
    mainLayout->addWidget(blackOnWhite);

    // Scheme 3: Green on Black (#11c70e on #000000)
    greenOnBlack = new QRadioButton("Green on Black", this);
    greenOnBlack->setStyleSheet("QRadioButton { color: #11c70e; background-color: black; padding: 5px; }");
    colorGroup->addButton(greenOnBlack, 2);
    mainLayout->addWidget(greenOnBlack);

    // Scheme 4: Yellow on Black (#f4d81e on #000000)
    yellowOnBlack = new QRadioButton("Yellow on Black", this);
    yellowOnBlack->setStyleSheet("QRadioButton { color: #f4d81e; background-color: black; padding: 5px; }");
    colorGroup->addButton(yellowOnBlack, 3);
    mainLayout->addWidget(yellowOnBlack);

    // Set default selection
    whiteOnBlack->setChecked(true);

    // Add spacer
    mainLayout->addStretch();

    // Create button layout for OK and Cancel
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &ColorSelectDialog::onOKClicked);
    buttonLayout->addWidget(okButton);

    cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, &QPushButton::clicked, this, &ColorSelectDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

int ColorSelectDialog::getSelectedSchemeIndex() const
{
    // Return the ID of the checked button in the group
    return colorGroup->checkedId();
}

void ColorSelectDialog::setDefaultScheme(int index)
{
    // Set the radio button at the given index as checked
    if (index >= 0 && index < 4) {
        QAbstractButton *button = colorGroup->button(index);
        if (button) {
            button->setChecked(true);
        }
    }
}

void ColorSelectDialog::onOKClicked()
{
    // Save the selected index
    selectedIndex = colorGroup->checkedId();

    // Accept the dialog (closes with Accepted result)
    accept();
}

void ColorSelectDialog::onCancelClicked()
{
    // Reject the dialog (closes with Rejected result)
    reject();
}
