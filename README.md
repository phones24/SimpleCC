<div align="center">

# SimpleCC

### MIDI CC Modulation VST3 Plugin

*By Randomware Audio*

[![Build Status](https://github.com/phones24/SimpleCC/actions/workflows/build.yml/badge.svg)](https://github.com/phones24/SimpleCC/actions)
[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/phones24/SimpleCC/releases)
[![License](https://img.shields.io/badge/license-Proprietary-red.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey.svg)](#download)

![SimpleCC Plugin Screenshot](media/simplecc.png)

---

**Simple. Powerful. Essential.**

</div>

## 🎯 Motivation

**SimpleCC** was created for DAWs that don't provide MIDI CC automation out of the box—like **Bitwig Essentials**. If your DAW limits your ability to automate hardware synthesizers or external MIDI gear, SimpleCC bridges that gap by giving you 16 modulation slots to send precise MIDI CC messages to your external instruments.

No more workarounds. No more limitations. Just pure, straightforward MIDI CC control.

---

## ✨ Features

- **16 Modulation Slots** - Control up to 16 different CC parameters simultaneously
- **Full MIDI Channel Support** - Send CC messages to any MIDI channel (1-16)
- **Preset System** - 26+ built-in presets for popular synthesizers (Moog, Roland, Korg, etc.)
- **User Presets** - Save and load your own custom CC mappings
- **Activity Indicators** - Visual feedback for each active modulation slot
- **Lightweight & Efficient** - Minimal CPU usage, sends CC only on value changes
- **Clean Interface** - Simple, intuitive design focused on workflow

---

## 🎹 Supported Instruments

SimpleCC includes factory presets for:

- **Access** - Virus TI
- **Arturia** - MicroFreak, MiniFreak
- **ASM** - Hydrasynth
- **Dreadbox** - Nymphes
- **Elektron** - Digitone, Analog Four
- **Korg** - Minilogue XD, Prologue
- **Modal** - Argon8, Cobalt8
- **Moog** - Subsequent 37, Matriarch, Grandmother
- **Nord** - Lead A1, Stage 3
- **Novation** - Peak, Summit
- **Oberheim** - OB-6
- **Roland** - JD-Xi, JD-XA, S-1, System-8
- **Sequential** - Prophet Rev2, Prophet-6, OB-6
- **Waldorf** - Blofeld, Iridium
- **Yamaha** - Reface CS, Reface DX

*Can't find your synth? Simply create and save your own preset!*

---

## 📥 Download

### Latest Release

Download the latest version from the [Releases](https://github.com/phones24/SimpleCC/releases) page:

- **Linux**: `SimpleCC-v1.0.0-Linux-VST3.zip`
- **Windows**: `SimpleCC-v1.0.0-Windows-VST3.zip`

### Development Builds (Artifacts)

Want the latest bleeding-edge features? Download build artifacts from the [Actions](https://github.com/phones24/SimpleCC/actions) tab:

1. Go to [GitHub Actions](https://github.com/phones24/SimpleCC/actions)
2. Click on the latest successful workflow run
3. Scroll down to the **Artifacts** section
4. Download `SimpleCC-v1.0.0-Linux-VST3` or `SimpleCC-v1.0.0-Windows-VST3`

> **Note**: Artifacts are automatically built from the latest code on every push to `main`. They represent the most recent development version and may contain unreleased features or bug fixes.

---

## 🔧 Installation

### Linux

```bash
# Extract the downloaded zip
unzip SimpleCC-v1.0.0-Linux-VST3.zip

# Copy to your VST3 directory
cp -r SimpleCC.vst3 ~/.vst3/

# Rescan plugins in your DAW
```

### Windows

```powershell
# Extract the downloaded zip
Expand-Archive SimpleCC-v1.0.0-Windows-VST3.zip

# Copy to your VST3 directory
Copy-Item -Path "SimpleCC.vst3" -Destination "$env:COMMONPROGRAMFILES\VST3" -Recurse

# Rescan plugins in your DAW
```

---

## 🚀 Quick Start

1. **Load SimpleCC** as a MIDI effect in your DAW
2. **Route MIDI output** to your hardware synthesizer
3. **Choose a preset** from the dropdown (or start with defaults)
4. **Enable slots** by checking the boxes next to the slots you want to use
5. **Map CC parameters** in your DAW's modulation system to SimpleCC's parameters
6. **Automate away!** Watch the activity indicators light up as you modulate

### Example: Modulating a Moog Subsequent 37

1. Load the **Moog Subsequent 37** preset
2. Enable Slot 1 (Filter Cutoff, CC 74)
3. Enable Slot 5 (Env Decay, CC 75)
4. In Bitwig, modulate SimpleCC's **Slot 1** parameter with an LFO
5. Automate **Slot 5** in the timeline
6. Enjoy your animated hardware synth!

---

## 🛠️ Building from Source

### Prerequisites

**Linux:**
- CMake 3.22+
- GCC/Clang with C++17 support
- Development packages: ALSA, X11, FreeType, Mesa

**Windows:**
- CMake 3.22+
- Visual Studio 2019 or later
- Git

### Build Instructions

#### Linux

```bash
chmod +x build.sh
./build.sh Release
```

The VST3 will be in `build/SimpleCC_artefacts/Release/VST3/SimpleCC.vst3`

#### Windows

```powershell
.\build.ps1 Release
```

The VST3 will be in `build\SimpleCC_artefacts\Release\VST3\SimpleCC.vst3`

---

## 📖 Technical Details

- **Format**: VST3
- **Type**: MIDI Effect / MIDI Processor
- **MIDI I/O**: MIDI Output only (no input processing)
- **Parameters**: 48 automatable parameters (16 slots × 3 controls)
- **State Saving**: Full DAW project state support
- **Framework**: JUCE 7.0.9
- **Build System**: CMake 3.22+

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

### Development Workflow

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📜 License

Copyright © 2026 Randomware Audio. All rights reserved.

This software is proprietary. Distribution, modification, or commercial use requires explicit permission from the copyright holder.

---

## 🙏 Acknowledgments

- Built with [JUCE](https://juce.com/) framework
- Inspired by the need for simple, reliable MIDI CC automation
- Dedicated to hardware synth enthusiasts everywhere

---

## 📧 Support

Found a bug? Have a feature request? 

- **Issues**: [GitHub Issues](https://github.com/phones24/SimpleCC/issues)
- **Discussions**: [GitHub Discussions](https://github.com/phones24/SimpleCC/discussions)

---

<div align="center">

**Made with ❤️ by Randomware Audio**

[Download](https://github.com/phones24/SimpleCC/releases) • [Documentation](https://github.com/phones24/SimpleCC) • [Report Bug](https://github.com/phones24/SimpleCC/issues)

</div>
