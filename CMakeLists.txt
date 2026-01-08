cmake_minimum_required(VERSION 3.16)
project(VideoOCR VERSION 1.0.0 LANGUAGES CXX)

# ===================== C++ Standard =====================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===================== Qt Auto Features =====================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ===================== Qt6 =====================
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Multimedia
    MultimediaWidgets
)

# ===================== OpenCV =====================
find_package(OpenCV REQUIRED)

# ===================== OpenGL (required by Qt6Gui) =====================
find_package(OpenGL REQUIRED)

# ===================== Tesseract =====================
# Linux DOES NOT provide TesseractConfig.cmake
# So we must use pkg-config on Linux
if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(TESSERACT REQUIRED tesseract)

    include_directories(${TESSERACT_INCLUDE_DIRS})
    link_directories(${TESSERACT_LIBRARY_DIRS})
else()
    # Windows / macOS (Homebrew provides config)
    find_package(Tesseract REQUIRED)
endif()

# ===================== Sources =====================
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

# ===================== Executable =====================
add_executable(VideoOCR ${PROJECT_SOURCES})

# ===================== Include Directories =====================
target_include_directories(VideoOCR PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${TESSERACT_INCLUDE_DIRS}
)

# ===================== Link Libraries =====================
target_link_libraries(VideoOCR PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    OpenGL::GL
    ${OpenCV_LIBS}
    ${TESSERACT_LIBRARIES}
)

# ===================== Platform Specific =====================
if(WIN32)
    set_target_properties(VideoOCR PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if(APPLE)
    set_target_properties(VideoOCR PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.videocr
    )
endif()

# ===================== Install =====================
include(GNUInstallDirs)
install(TARGETS VideoOCR
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
