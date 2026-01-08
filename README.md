# Video OCR Application

A cross-platform Qt6 application that performs Optical Character Recognition (OCR) on live video streams from webcam or internal camera.

## Features

- **Live Video Capture**: Supports webcam and internal camera input
- **Real-time Processing**: Converts video frames to monochrome images
- **Multiple Color Schemes**: 4 predefined color schemes for optimal OCR
  - White on Black (#ffffff / #000000)
  - Black on White (#000000 / #ffffff)
  - Green on Black (#11c70e / #000000)
  - Yellow on Black (#f4d81e / #000000)
- **OCR Recognition**: Uses Tesseract OCR engine for text recognition
- **F4 Hotkey**: Quick capture and OCR with a single keypress
- **Multi-threaded**: OCR processing runs in separate thread to prevent UI freezing

## Prerequisites

### Required Software

1. **Qt 6.5.3** (Open Source)
   - Download from: https://www.qt.io/download-qt-installer
   - Select Qt 6.5.3 during installation

2. **CMake 3.16+**
   - Download from: https://cmake.org/download/

3. **C++ Compiler**
   - Windows: Visual Studio 2019/2022 with C++ desktop development
   - Linux: GCC 9+ or Clang 10+
   - macOS: Xcode Command Line Tools

### Required Libraries

#### OpenCV
**Windows:**
```bash
# Download prebuilt binaries from:
# https://opencv.org/releases/
# Extract and set OpenCV_DIR environment variable to:
# C:\opencv\build
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install libopencv-dev
```

**macOS:**
```bash
brew install opencv
```

#### Tesseract OCR
**Windows:**
```bash
# Download installer from:
# https://github.com/UB-Mannheim/tesseract/wiki
# Install to default location: C:\Program Files\Tesseract-OCR
# Download tessdata from:
# https://github.com/tesseract-ocr/tessdata
# Place eng.traineddata in C:\Program Files\Tesseract-OCR\tessdata\
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install tesseract-ocr libtesseract-dev
```

**macOS:**
```bash
brew install tesseract
```

## Project Structure

```
VideoOCR/
├── CMakeLists.txt              # Build configuration
├── main.cpp                    # Application entry point
├── mainwindow.h/cpp           # Main application window
├── videoprocessor.h/cpp       # Video frame processing and OCR
├── colorselectdialog.h/cpp    # Color scheme selection dialog
├── ocrresultdialog.h/cpp      # OCR results display dialog
└── README.md                   # This file
```

## Building the Project

### Step 1: Create Project Directory
```bash
mkdir VideoOCR
cd VideoOCR
```

### Step 2: Copy Source Files
Place all the provided source files in the VideoOCR directory:
- CMakeLists.txt
- main.cpp
- mainwindow.h, mainwindow.cpp
- videoprocessor.h, videoprocessor.cpp
- colorselectdialog.h, colorselectdialog.cpp
- ocrresultdialog.h, ocrresultdialog.cpp

### Step 3: Create Build Directory
```bash
mkdir build
cd build
```

### Step 4: Configure with CMake

**Windows (Visual Studio):**
```bash
cmake .. -G "Visual Studio 17 2022" -A x64 ^
  -DCMAKE_PREFIX_PATH="C:\Qt\6.5.3\msvc2019_64" ^
  -DOpenCV_DIR="C:\opencv\build" ^
  -DTesseract_INCLUDE_DIRS="C:\Program Files\Tesseract-OCR\include" ^
  -DTesseract_LIBRARIES="C:\Program Files\Tesseract-OCR\lib\tesseract50.lib"
```

**Linux:**
```bash
cmake .. \
  -DCMAKE_PREFIX_PATH="/path/to/Qt/6.5.3/gcc_64" \
  -DCMAKE_BUILD_TYPE=Release
```

**macOS:**
```bash
cmake .. \
  -DCMAKE_PREFIX_PATH="/path/to/Qt/6.5.3/macos" \
  -DCMAKE_BUILD_TYPE=Release
```

### Step 5: Build the Project

**Windows:**
```bash
cmake --build . --config Release
```

**Linux/macOS:**
```bash
cmake --build . -j4
```

### Step 6: Prepare Tesseract Data

Create a `tessdata` folder in your build directory (or application directory) and copy the English trained data:

**Windows:**
```bash
mkdir tessdata
copy "C:\Program Files\Tesseract-OCR\tessdata\eng.traineddata" tessdata\
```

**Linux/macOS:**
```bash
mkdir tessdata
cp /usr/share/tesseract-ocr/4.00/tessdata/eng.traineddata tessdata/
```

Alternatively, set the `TESSDATA_PREFIX` environment variable:
```bash
export TESSDATA_PREFIX=/usr/share/tesseract-ocr/4.00/tessdata/
```

## Running the Application

**Windows:**
```bash
cd Release
VideoOCR.exe
```

**Linux:**
```bash
./VideoOCR
```

**macOS:**
```bash
open VideoOCR.app
```

## Usage Guide

### Starting the Application
1. Launch the application
2. Click "Start Camera" to begin video capture
3. The live video feed will appear in the main window

### Selecting Color Scheme
1. Use the "Color Scheme" dropdown to select your preferred monochrome color scheme
2. The video processing will update in real-time

### Performing OCR
1. Ensure the camera is active
2. Position text in front of the camera
3. Press **F4** key to capture the current frame
4. OCR processing will begin (may take a few seconds)
5. Results will appear in a separate dialog window

### OCR Results Window
- **View Text**: See the recognized text
- **Copy to Clipboard**: Copy the text for use elsewhere
- **Clear**: Clear the current text
- **Close**: Close the results window

## Troubleshooting

### Camera Not Working
- **Permission Issues**: Ensure the application has camera access permissions
- **Multiple Cameras**: Try disconnecting other cameras if multiple are detected

### Tesseract Errors
```
Error: Could not initialize Tesseract API
```
**Solution**: 
- Verify tessdata folder exists with eng.traineddata
- Set TESSDATA_PREFIX environment variable
- Check Tesseract installation path

### OpenCV Not Found
```
CMake Error: Could not find OpenCV
```
**Solution**:
- Set OpenCV_DIR to the OpenCV build directory
- On Windows: `set OpenCV_DIR=C:\opencv\build`
- Verify OpenCV installation

### Qt Not Found
```
CMake Error: Could not find Qt6
```
**Solution**:
- Set CMAKE_PREFIX_PATH to Qt installation directory
- Example: `set CMAKE_PREFIX_PATH=C:\Qt\6.5.3\msvc2019_64`

### Poor OCR Accuracy
- Ensure good lighting conditions
- Use high contrast color schemes
- Position text clearly in frame
- Avoid motion blur (hold camera steady)

## Advanced Configuration

### Changing OCR Language
Edit `videoprocessor.cpp`, line 31:
```cpp
// Change "eng" to your desired language code
if (tessApi->Init(nullptr, "eng")) {
```

Available languages: https://github.com/tesseract-ocr/tessdata

### Modifying Color Schemes
Edit `mainwindow.cpp`, lines 29-32:
```cpp
colorSchemes.append({"Custom Name", QColor("#foreground"), QColor("#background")});
```

### Adjusting Threshold
Edit `videoprocessor.cpp`, line 196:
```cpp
// Change threshold value (0-255)
cv::threshold(gray, binary, 128, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
```

## Performance Optimization

### For Low-End Systems
1. Reduce video resolution in camera settings
2. Increase OCR processing interval
3. Use simpler color schemes

### For High-End Systems
1. Enable GPU acceleration in OpenCV (requires rebuild with CUDA)
2. Use multiple Tesseract instances for parallel processing

## Platform-Specific Notes

### Windows
- Requires Visual C++ Redistributable
- May need to copy DLLs to executable directory:
  - Qt6Core.dll, Qt6Gui.dll, Qt6Widgets.dll, Qt6Multimedia.dll
  - opencv_world4XX.dll
  - tesseract50.dll

### Linux
- Requires camera access permissions
- May need to add user to `video` group: `sudo usermod -a -G video $USER`

### macOS
- Requires camera permission in System Preferences > Security & Privacy
- May need to sign the application for distribution

### Android/iOS
- Requires Qt for mobile licensing
- Additional platform-specific implementations needed
- Camera access requires AndroidManifest.xml / Info.plist permissions

## Contributing

Feel free to submit issues and enhancement requests!

## License

This project uses:
- Qt6 (LGPL/GPL)
- OpenCV (Apache 2.0)
- Tesseract OCR (Apache 2.0)

## Credits

Developed using:
- Qt Framework 6.5.3
- OpenCV 4.x
- Tesseract OCR 5.x

## Contact & Support

For issues and questions, please refer to:
- Qt Documentation: https://doc.qt.io/
- OpenCV Documentation: https://docs.opencv.org/
- Tesseract Documentation: https://tesseract-ocr.github.io/
