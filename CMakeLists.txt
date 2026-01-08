cmake_minimum_required(VERSION 3.16)
project(VideoOCR VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++17 (required for Qt6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic MOC, UIC, and RCC processing for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required Qt6 packages
# Qt6::Core - Core non-GUI functionality
# Qt6::Widgets - GUI widgets
# Qt6::Multimedia - Camera and video handling
# Qt6::MultimediaWidgets - Video display widgets
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia MultimediaWidgets)

# Find OpenCV package for image processing
find_package(OpenCV REQUIRED)

# Find Tesseract OCR library
# On Windows: Install from https://github.com/UB-Mannheim/tesseract/wiki
# On Linux: sudo apt-get install tesseract-ocr libtesseract-dev
# On macOS: brew install tesseract
find_package(Tesseract REQUIRED)

# If Tesseract is not found via find_package, you can manually set:
# set(Tesseract_INCLUDE_DIRS "C:/Program Files/Tesseract-OCR/include")
# set(Tesseract_LIBRARIES "C:/Program Files/Tesseract-OCR/lib/tesseract50.lib")

# Source files for the application
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    videoprocessor.cpp
    videoprocessor.h
    colorselectdialog.cpp
    colorselectdialog.h
    ocrresultdialog.cpp
    ocrresultdialog.h
)

# Create the executable
add_executable(VideoOCR ${PROJECT_SOURCES})

# Link Qt libraries
target_link_libraries(VideoOCR PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)

# Link OpenCV libraries
target_link_libraries(VideoOCR PRIVATE ${OpenCV_LIBS})

# Link Tesseract OCR library
target_link_libraries(VideoOCR PRIVATE ${Tesseract_LIBRARIES})

# Include directories for header files
target_include_directories(VideoOCR PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${Tesseract_INCLUDE_DIRS}
)

# Platform-specific settings
if(WIN32)
    # Windows: Set subsystem to windows (not console)
    set_target_properties(VideoOCR PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if(APPLE)
    # macOS: Set bundle properties
    set_target_properties(VideoOCR PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.videocr
    )
endif()

# Install target
install(TARGETS VideoOCR
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
