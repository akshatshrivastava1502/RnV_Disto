# GEMINI.md

# Othrys - Project Context

## Overview

Othrys is a professional creative audio effects plugin built with JUCE and modern C++.

Unlike a traditional distortion plugin, Othrys is designed as a modular multi-effects processor targeted primarily at producers working in Hip-Hop, Trap, Drill, Pop, EDM, and other modern genres.

The project emphasizes:

- Professional sound quality
- Modular DSP architecture
- Low CPU usage
- Real-time safe processing
- Commercial-quality code
- Maintainability
- Expandability

The long-term goal is to create a polished commercial plugin rather than a proof-of-concept.

---

# Tech Stack

Framework
- JUCE

Language
- C++20

Plugin Format
- VST3

Future Targets
- AU
- Standalone

Version Control
- Git

---

# Project Philosophy

Every feature should satisfy these principles:

- Audible improvements over unnecessary complexity.
- Modular DSP.
- Real-time safety.
- Clean architecture.
- Readable code.
- Easy future expansion.

No feature should compromise stability or maintainability.

---

# Current Project Status

Implemented:

✅ Distortion

✅ Noise Gate

✅ Delay

✅ Reverb

✅ Tone Stack

✅ Oversampling

✅ APVTS Parameter System

✅ Custom LookAndFeel

✅ Preset Foundation

In Progress:

🟨 Graphic EQ

Future:

⬜ Analyzer

⬜ Advanced Preset Browser

⬜ CPU Optimization

⬜ UI Polish

⬜ Additional DSP Modes

---

# Current DSP Chain

Current signal flow is conceptually:

Input

↓

Input Gain

↓

Noise Gate

↓

DC Blocker

↓

Oversampling

↓

Distortion

↓

Tone Stack

↓

Delay

↓

Reverb

↓

Output Gain

↓

Output

The Graphic EQ will eventually replace or complement the current Tone Stack.

The DSP chain should remain modular so effects can be reordered or expanded in future versions.

---

# Planned Graphic EQ

The Graphic EQ is the current development priority.

Design goals:

- 10-band graphic equalizer
- Inspired by classic hardware graphic EQ workflows
- Simple and fast to use
- Smooth automation
- Zero zipper noise
- Low CPU overhead

Suggested layout:

HPF

65 Hz

125 Hz

250 Hz

500 Hz

1 kHz

2 kHz

4 kHz

8 kHz

16 kHz

LPF

Each band should support:

- ±12 dB gain
- Parameter smoothing
- Independent processing
- Automation support

The EQ should integrate naturally into the existing DSP chain without tightly coupling to other modules.

---

# Repository Structure

Assets/

Contains:

- Images
- Knob assets
- Background artwork
- LED assets

No DSP logic belongs here.

---

Source/

Contains all source code.

Current major modules:

DSP/

Independent DSP processors.

Current processors include:

- Distortion
- Delay
- Reverb
- Noise Gate
- DC Blocker
- Tone Stack

DSP modules should remain self-contained.

No GUI code belongs inside DSP modules.

---

GUI/

Contains:

- Custom LookAndFeel
- Drawing code
- Component styling

GUI must never contain DSP algorithms.

---

Parameters/

Contains parameter definitions.

ParameterDefines.h acts as the single source of truth for plugin parameters.

New parameters should be added here before being referenced elsewhere.

---

PluginProcessor

Responsibilities:

- Own DSP modules
- Prepare processors
- Process audio
- Manage parameter updates
- Save/load plugin state
- Coordinate the DSP chain

---

PluginEditor

Responsibilities:

- User interface
- Layout
- Attachments
- Painting
- User interaction

No DSP calculations belong here.

---

# DSP Design Principles

Every DSP module should:

- Be independent.
- Have a clear responsibility.
- Be reusable.
- Avoid hidden dependencies.

Modules should expose a predictable lifecycle:

- prepare()
- reset()
- process()
- updateParameters()

Whenever practical, DSP classes should be usable independently of the plugin.

---

# Parameter Management

The project uses AudioProcessorValueTreeState (APVTS).

Parameter IDs originate in ParameterDefines.h.

Parameter pointers are cached rather than repeatedly queried inside processBlock().

Future parameter additions should follow the existing APVTS architecture.

---

# Current GUI Direction

The UI is intentionally minimal and hardware-inspired.

Design language:

- Dark theme
- Physical hardware influence
- Large controls
- Minimal text
- Clear visual hierarchy

Future UI improvements should preserve this aesthetic.

---

# Performance Goals

Othrys is intended to run comfortably on large production sessions.

Priorities:

- Low CPU usage
- Low latency
- Smooth automation
- Stable processing
- Efficient memory usage

Performance regressions should be avoided.

---

# Current Development Priorities

Priority 1

Complete Graphic EQ.

Priority 2

Improve preset management.

Priority 3

Improve parameter smoothing consistency.

Priority 4

Optimize DSP performance.

Priority 5

Improve UI polish.

Priority 6

Expand factory presets.

---

# Future Architecture

As the project grows, DSP processing should migrate toward a dedicated EffectsChain (or DSPChain) object.

Rather than PluginProcessor owning every DSP stage directly, the processor should delegate audio processing to a centralized chain.

Conceptually:

PluginProcessor

↓

EffectsChain

├── Noise Gate

├── DC Blocker

├── Distortion

├── Graphic EQ

├── Delay

├── Reverb

↓

Output

This separation improves readability, testing, scalability, and future feature development.

---

# Long-Term Vision

Othrys should evolve into a commercial-quality creative effects plugin with:

- Professional DSP
- Modern UI
- Stable architecture
- Expandable effect chain
- Efficient codebase
- Excellent user experience

Every contribution should move the project closer to that goal without sacrificing maintainability.