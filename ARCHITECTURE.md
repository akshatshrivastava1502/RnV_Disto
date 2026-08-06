# ARCHITECTURE.md

# Othrys Architecture

## Purpose

This document defines the architectural principles of the Othrys project.

Unlike implementation details, the architecture described here is intended to remain stable as the project evolves.

Folder names, file names, class names, and internal implementations may change over time. The architectural goals described here should continue to guide future development.

---

# Design Philosophy

Othrys is a modular creative audio effects plugin.

Its architecture prioritizes:

- Modularity
- Maintainability
- Real-time safety
- Testability
- Expandability
- Readability

The system should remain easy to extend with additional DSP modules without requiring major architectural rewrites.

---

# Architectural Principles

Every subsystem should have a clearly defined responsibility.

Avoid tightly coupled code.

Favor composition over inheritance.

Avoid global state.

Minimize dependencies between modules.

Every component should be replaceable with minimal impact on the rest of the project.

---

# High-Level System

Conceptually the project consists of five major layers.

```
User Interface

↓

Parameter System

↓

Plugin Core

↓

DSP Processing

↓

Audio Output
```

Each layer should only communicate with adjacent layers whenever practical.

---

# Core Responsibilities

## User Interface

Responsible for:

- Rendering
- User interaction
- Parameter attachment
- Visual feedback

The UI must never perform DSP processing.

The UI should communicate exclusively through the parameter system.

---

## Parameter System

Acts as the bridge between UI and DSP.

Responsibilities include:

- Parameter definitions
- State serialization
- Automation
- Preset integration
- Value synchronization

The parameter system should remain the single source of truth for user-adjustable values.

---

## Plugin Core

The processor coordinates the application.

Responsibilities include:

- Preparing DSP
- Managing plugin lifecycle
- Processing audio
- Updating DSP modules
- Saving and restoring state
- Managing processing order

The processor should avoid containing DSP algorithms directly whenever practical.

---

## DSP Layer

The DSP layer contains all audio processing.

Examples include:

- Distortion
- Noise Gate
- Equalization
- Delay
- Reverb
- Filters
- Utility processors

Each DSP module should:

- Have one primary responsibility
- Be reusable
- Be independently testable
- Avoid GUI dependencies
- Avoid knowledge of unrelated DSP modules

DSP modules should communicate through the processing chain rather than directly with one another.

---

# Signal Flow

The exact processing chain may evolve.

Conceptually, Othrys follows:

```
Input

↓

Pre-processing

↓

Dynamic Processing

↓

Nonlinear Processing

↓

Frequency Shaping

↓

Time-based Effects

↓

Output Processing

↓

Output
```

The implementation may reorder stages as needed for better sound quality or performance.

Architecture should support this flexibility.

---

# DSP Chain

The DSP chain should behave like a pipeline.

Example:

```
Input

↓

Noise Gate

↓

Distortion

↓

Graphic EQ

↓

Delay

↓

Reverb

↓

Output
```

The exact chain is not fixed.

Future modules may be added without redesigning the architecture.

Examples include:

- Compressor
- Chorus
- Limiter
- Saturation
- Stereo Imaging
- Multiband Processing

The architecture should support adding, removing, replacing, or reordering DSP stages with minimal code changes.

---

# Module Independence

Every processing module should:

- own its own state
- expose a predictable interface
- avoid hidden dependencies
- avoid modifying unrelated modules

Preferred lifecycle:

```
prepare()

reset()

updateParameters()

process()
```

Individual implementations may expose additional helper functions where appropriate.

---

# Processing Contracts

Every DSP module should satisfy these expectations.

Input:

Receives valid audio buffers prepared by the processor.

Output:

Produces processed audio without modifying unrelated state.

Timing:

Safe for repeated real-time execution.

Memory:

Avoid allocations during processing.

Thread Safety:

Safe for use within the real-time audio thread.

---

# Data Flow

Information should generally move in one direction.

```
User

↓

GUI

↓

Parameters

↓

Plugin Processor

↓

DSP Modules

↓

Audio Output
```

Reverse dependencies should be avoided.

DSP should never directly manipulate GUI components.

---

# Folder Organization

The physical folder structure may change over time.

Regardless of layout, responsibilities should remain separated.

Suggested logical organization:

- DSP
- GUI
- Parameters
- Assets
- Utilities
- Presets
- Resources

New folders may be introduced as needed.

---

# Dependency Rules

Higher-level systems may depend on lower-level systems.

Lower-level systems should not depend on higher-level systems.

Example:

GUI

↓

Parameters

↓

DSP

Not the reverse.

Whenever possible:

GUI → DSP should occur only through parameters.

---

# Parameter Architecture

All user-adjustable values should originate from a centralized parameter system.

DSP modules should consume parameters rather than owning independent copies whenever possible.

Automation, preset loading, and state restoration should operate through this shared parameter layer.

---

# Error Handling

Audio processing should avoid exceptions.

Recoverable errors should be handled gracefully.

Programming errors should be addressed during development rather than at runtime.

---

# Performance Goals

Architectural decisions should prioritize:

- Low CPU usage
- Predictable execution
- Minimal allocations
- Efficient cache usage
- Real-time safety

Performance improvements should never significantly reduce readability unless justified.

---

# Extensibility

The architecture should support future expansion without major redesign.

Potential future additions include:

- Additional DSP processors
- Alternative processing chains
- Parallel processing
- Mid/Side processing
- Multiband processing
- Analyzer components
- Preset management improvements
- Standalone application support

New functionality should integrate into existing architecture rather than bypassing it.

---

# Evolution Policy

This document intentionally describes architectural principles rather than implementation details.

As the project evolves:

- Folder structures may change.
- Class names may change.
- Processing order may change.
- Internal implementations may change.

When making architectural changes:

- Preserve modularity.
- Preserve separation of responsibilities.
- Preserve real-time safety.
- Preserve maintainability.

If implementation changes require updates to this document, update the principles rather than documenting temporary implementation details.

Architecture should describe *why the system is organized the way it is*, not merely *how it is currently implemented*.