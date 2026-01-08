/*
 * main.cpp - Application Entry Point
 *
 * Purpose: Initialize the Qt application and display the main window
 * This is the starting point of the application
 */

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // Create the Qt application object
    // This manages application-wide resources and event loop
    QApplication app(argc, argv);

    // Set application metadata (useful for settings storage)
    app.setApplicationName("Video OCR");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("YourOrganization");

    // Create and show the main window
    MainWindow window;
    window.show();

    // Start the Qt event loop
    // This keeps the application running and processes events
    return app.exec();
}
