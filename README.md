# EchoReverb Project

This repository contains the **firmware, hardware designs, documentation, and simulations** for a digital echo and reverb effects processor built using **Teensy 4.1 + SGTL5000 Audio Shield**.

## 📂 Repository Structure
- **firmware/** → Arduino sketches (.ino) for Teensy
- **docs/** → project background, requirements, diagrams, and report
- **hardware/** → schematics, PCB layouts, part datasheets
- **simulations/** → MATLAB / Python DSP prototypes (delay, reverb)

## 🎯 Objectives
- Implement real-time **echo** and **reverb** with adjustable parameters.
- Ensure **low latency (<10 ms)** and **high fidelity audio**.
- Provide **user-friendly interface** with knobs, buttons, and OLED screen.
- Design hardware with **true bypass** and **robust analog I/O**.

## 🚀 Getting Started
1. Install **Arduino IDE 2.x** + **Teensyduino support**.
2. Select **Board: Teensy 4.1** and **USB Type: Serial + Audio**.
3. Open the sketch in `firmware/echo_reverb_minimal/` and compile.
4. Upload to Teensy with the **Audio Shield** attached.

## 🛠️ Hardware
- Teensy 4.1
- Teensy Audio Shield (SGTL5000)
- NE5532 op-amps
- DPDT relay (true bypass)
- Pots + buttons + OLED

## 📖 Documentation
See `/docs` for detailed background, system requirements, and diagrams.

