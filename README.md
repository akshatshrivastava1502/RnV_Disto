# Othrys Audio Plugin

Othrys is a modular creative audio effects plugin built with C++ and the JUCE framework. It is designed as a versatile multi-effects processor for music producers and sound designers, combining several essential effects into a single, cohesive unit.

## Core Features

The plugin currently includes the following DSP modules, each of which can be individually bypassed:

*   **Distortion:** A non-linear distortion unit with five distinct algorithms: Soft Clip, Hard Clip, Foldback, Tube, and Tape.
*   **Graphic EQ:** A 9-band graphic equalizer (from 65Hz to 16kHz) with dedicated High-Pass and Low-Pass filters for comprehensive frequency shaping.
*   **Reverb:** A spatial processor with "Room" and "Hall" algorithms.
*   **Delay:** A tempo-synced or free-time delay effect with feedback control.
*   **Noise Gate:** A dynamics processor to eliminate unwanted background noise.
*   **Tone Stack:** A simple, efficient two-control EQ for quick tonal adjustments post-distortion.

## Technology

*   **Language:** C++20
*   **Framework:** JUCE 8
*   **Build System:** CMake
*   **Plugin Formats:** VST3, AudioUnit (AU), Standalone

Dependencies such as the JUCE framework and the ASIO SDK (on Windows) are managed automatically by CMake's `FetchContent` module, requiring no manual installation.

## Architecture

Othrys is built on a modular architecture that emphasizes a clean separation of concerns between the user interface, parameter management, and audio processing.

*   **User Interface (UI):** The UI is responsible for all visual rendering and user interaction. It communicates with the audio engine exclusively through the parameter system and performs no audio processing itself. The interface is organized into tabs for each major effect module.

*   **Parameter System:** The plugin uses JUCE's `AudioProcessorValueTreeState` (APVTS) as the single source of truth for all user-adjustable parameters. This centralizes state management, ensuring robust automation, preset handling, and synchronization between the UI and the audio engine. All parameters are defined in `Source/Parameters/ParameterDefines.h`.

*   **DSP (Digital Signal Processing):** All audio processing logic is encapsulated in self-contained classes within the `Source/DSP/` directory. Each module (e.g., `GraphicEQ`, `Distortion`) is designed to be independent and reusable.

### Signal Flow

The audio signal is processed through the following chain:

1.  **Input Gain:** Adjusts the level of the incoming signal.
2.  **DC Blocker:** Removes any DC offset.
3.  **Noise Gate:** Attenuates the signal when it falls below a set threshold.
4.  **Distortion Stage:**
    *   The signal is up-sampled using a high-quality oversampler to reduce aliasing.
    *   The selected **Distortion** algorithm is applied.
    *   The signal is down-sampled back to the original sample rate.
    *   The **Tone Stack** provides immediate post-distortion shaping.
5.  **Graphic EQ:** The 9-band graphic equalizer is applied.
6.  **Time-Based Effects:** The **Delay** and **Reverb** modules process the signal.
7.  **Output Stage:**
    *   **Output Gain** adjusts the final level.
    *   A safety **Limiter** prevents digital clipping.
    *   A **Dry/Wet Mixer** blends the processed signal with the original, untouched input signal.

## How to Build

### Prerequisites

*   CMake (version 3.21 or higher)
*   A C++20 compatible compiler (e.g., MSVC on Windows, Clang on macOS, GCC/Clang on Linux)

### Build Steps

1.  Clone the repository to your local machine.
2.  Open a terminal or command prompt in the project's root directory.
3.  Configure the project using CMake:
    ```shell
    cmake -B build
    ```
4.  Compile the project:
    ```shell
    cmake --build build --config Release
    ```

Upon successful compilation, the plugin files will be located in the `build/Othrys_artefacts` directory. On Windows, a copy of the `Othrys.exe` standalone application will also be placed in the project's root directory for easy access.

## Folder Structure

*   `Assets/`: Contains all binary assets, such as images for the background, knobs, and switches.
*   `Source/`: Contains all C++ source code.
    *   `DSP/`: Houses the independent audio processing modules.
    *   `Parameters/`: Defines all automatable parameters for the plugin.
    *   `PluginProcessor.h/.cpp`: The core of the plugin, responsible for managing the DSP chain and plugin state.
    *   `PluginEditor.h/.cpp`: Implements the graphical user interface and its components.

## Preset System

The plugin features a comprehensive preset management system.

*   **Saving & Loading:** Presets are saved in a custom `.rnv` format, which is a human-readable JSON file. The system can also load a specific `.xml` preset format from another plugin for compatibility.
*   **Animated Transitions:** When loading a preset, the plugin's parameters smoothly animate to their new values, providing a seamless user experience without abrupt sonic changes.
*   **Location:** Presets are stored in a dedicated folder within the user's "Documents" directory (`Documents/ProdByRnV/Othrys/Presets`).
=======
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

## Changes Made So Far

During the development process, the following key changes and fixes were implemented to ensure the project builds successfully and adheres to modern JUCE practices:

*   **Updated Deprecated JUCE API:** Replaced the deprecated `juce::AudioProcessorValueTreeSystem` with the modern `juce::AudioProcessorValueTreeState` across the codebase, ensuring compatibility and proper parameter management.
*   **Resolved Type Conversion Warnings:** Addressed `double` to `float` conversion warnings and `size_t` to `juce::uint32` warnings by applying explicit `static_cast` operations, preventing them from being treated as errors.
*   **Fixed DSP Module API Mismatches:** Corrected the parameter signatures for `DCBlocker::prepare` and `DCBlocker::process` in `PluginProcessor.cpp` to match the `DCBlocker.h` definition.
*   **Silenced Unreferenced Parameter Warnings:** Implemented `juce::ignoreUnused()` for parameters that were intentionally not used in certain functions, resolving associated warnings.
*   **Corrected Syntax Errors:** Fixed minor syntax errors in `PluginEditor.cpp` related to accessing the `AudioProcessorValueTreeState` member.

## Future Updates

*   **Additional DSP Effects:** Implement new effects or refine existing ones (e.g., adding modulation, more complex filters).
*   **Enhanced GUI Features:** Develop a more sophisticated and interactive user interface.
*   **Comprehensive Testing:** Expand unit and integration tests to cover all DSP modules and plugin functionality.
*   **Cross-Platform Testing:** Verify full functionality on both Windows and macOS platforms.
*   **Documentation:** Add detailed API documentation for custom DSP modules.
