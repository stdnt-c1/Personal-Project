# ESP32-based Joystick Project

## Overview
This project implements a custom ESP32-based joystick with wireless capabilities and advanced features.

## Components List

### Core Components
- 1x ESP32 MINI Kit (Main microcontroller)
- 2x KY-023 Joystick Module (Dual-axis input)
- 8x Push Button (Additional controls)
- 4x Limit Switch (Trigger buttons)
- 1x 18650 Battery Shield (Power management)
- 2x 18650 Battery (Power source)
- 1x 0.66-inch White OLED Display Shield (Information display)
- 1x NRF24L01+PA/LNA RF Module (Wireless communication)
- 1x Passive Buzzer (Audio feedback)
- 1x 2S Battery Indicator (Power monitoring)

### Additional Components
- Capacitors
  - 1x 2200uF 16v Electrolytic (ESP32 bulk capacitor)
  - 1x 220uF 16v Electrolytic (RF module smoothing)
  - Multiple 10nF, 100nF (50v Tantalum) (RF decoupling)
  - Multiple 22pF, 100nF (50v Ceramic) (RF decoupling)
- Diodes
  - Multiple 1N4148 (200MA 100V Zener) (RF protection)
- Resistors
  - Metal Film Resistors (1/4w) for button matrix and voltage division

## Technical Implementation

### Joystick Voltage Management
- Converting 5V joystick output to 3.3V ESP32 input using voltage dividers
- Configuration: 2kΩ (to 5V) and 1kΩ (to ground) for 3.3V output

### Button Matrix Design
- 8 buttons organized in 2 groups of 4
- Resistor ladder implementation for GPIO pin optimization
- Voltage divider network:
  ```
  [3.3V]
     |
     +---[Button 1]---[1k]---+
     +---[Button 2]---[2.2k]-+
     +---[Button 3]---[3.3k]-+---> [Analog Input]
     +---[Button 4]---[4.7k]-+
                             |
                           [10k]
                             |
                            GND
  ```

### Interface Connections
- Trigger Switches: Direct GPIO connection for simultaneous input
- OLED Display: I2C interface
- RF Module: SPI interface
- Buzzer: PWM control
- Battery Monitoring: Direct battery connection

## Project Status
Currently in planning phase. Implementation will be done in stages:
1. Core hardware setup
2. Basic input handling
3. Wireless communication
4. UI and feedback systems
5. Power management
6. Testing and refinement
