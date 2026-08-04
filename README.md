# Othrys Audio Plugin

Othrys is a digital audio workstation (DAW) plugin, developed using the JUCE framework and CMake. It is designed to provide powerful distortion processing capabilities, complemented by a suite of essential digital signal processing (DSP) modules. The plugin can be built as a VST3, Audio Unit (AU), and a standalone application, offering flexibility for various audio production environments.

## Features

*   **Distortion:** Core distortion processing with adjustable parameters.
*   **DC Blocker:** Removes unwanted DC offset from the audio signal.
*   **Noise Gate:** Suppresses noise below a specified threshold.
*   **Tone Stack:** Provides tonal shaping capabilities (HPF, LPF).
*   **Dry/Wet Mixer:** Blends the processed signal with the original dry signal.

## How to Build and Use

### Prerequisites

Before building Othrys, ensure you have the following installed:

*   **CMake (>= 3.21):** For generating build system files.
*   **Git:** For cloning the repository and managing dependencies.
*   **C++ Compiler:**
    *   **Windows:** Visual Studio (2022 or later recommended).
    *   **macOS:** Xcode Command Line Tools.

### Building the Plugin

1.  **Clone the Repository:**
    ```bash
    git clone <repository_url>
    cd Othrys-main
    ```
    (Replace `<repository_url>` with the actual repository URL)

2.  **Clean Previous Build (if any):**
    It's recommended to start with a clean build directory.
    ```bash
    Remove-Item -Path "build" -Recurse -Force # On Windows (PowerShell)
    # rm -rf build # On macOS/Linux
    ```

3.  **Generate Build Files with CMake:**
    This command configures the project and creates the build files (e.g., Visual Studio solution files or Xcode project files) in a new `build` directory.
    ```bash
    cmake -B build -S .
    ```

4.  **Compile the Project:**
    This command compiles the project in Release mode.
    ```bash
    cmake --build build --config Release
    ```

5.  **Locate the Plugin/Application:**
    After a successful build, the generated plugin binaries (`.vst3`, `.component`, etc.) and the standalone application (`.exe` on Windows, `.app` on macOS) will be located in the `build/Othrys_artefacts/Release` directory, within their respective subfolders (e.g., `VST3`, `Standalone`).

    *   **VST3/AU:** Copy the generated plugin files to your DAW's plugin folders.
    *   **Standalone:** Run the executable directly.

## Changes Made So Far (by the AI Agent)

During the development process, the following key changes and fixes were implemented to ensure the project builds successfully and adheres to modern JUCE practices:

*   **Updated Deprecated JUCE API:** Replaced the deprecated `juce::AudioProcessorValueTreeSystem` with the modern `juce::AudioProcessorValueTreeState` across the codebase, ensuring compatibility and proper parameter management.
*   **Resolved Type Conversion Warnings:** Addressed `double` to `float` conversion warnings and `size_t` to `juce::uint32` warnings by applying explicit `static_cast` operations, preventing them from being treated as errors.
*   **Fixed DSP Module API Mismatches:** Corrected the parameter signatures for `DCBlocker::prepare` and `DCBlocker::process` in `PluginProcessor.cpp` to match the `DCBlocker.h` definition.
*   **Silenced Unreferenced Parameter Warnings:** Implemented `juce::ignoreUnused()` for parameters that were intentionally not used in certain functions, resolving associated warnings.
*   **Corrected Syntax Errors:** Fixed minor syntax errors in `PluginEditor.cpp` related to accessing the `AudioProcessorValueTreeState` member.

## Future Updates

*   **Complete Renaming:** Fully transition all instances of "RnVDisto" (including classes, namespaces, internal identifiers, preset folder names, and file extensions like `.rnv`) to "Othrys" throughout the project, including updates to `CMakeLists.txt` for bundle identifiers and plugin codes.
*   **Additional DSP Effects:** Implement new effects or refine existing ones (e.g., adding modulation, more complex filters).
*   **Enhanced GUI Features:** Develop a more sophisticated and interactive user interface.
*   **Comprehensive Testing:** Expand unit and integration tests to cover all DSP modules and plugin functionality.
*   **Cross-Platform Testing:** Verify full functionality on both Windows and macOS platforms.
*   **Documentation:** Add detailed API documentation for custom DSP modules.