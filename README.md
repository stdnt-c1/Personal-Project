# ESP32-based Joystick Project

## Overview
This project outlines the design and planning of a **custom-built ESP32-based joystick** controller intended for versatile applications—ranging from gaming to RC control. It features analog joystick modules, button inputs, wireless communication, and onboard feedback through display and sound.

**Note:** This project is currently in the *planning and prototyping phase*. All physical prototyping is being done using **protoboards**, so comfort and layout are optimized for personal usability.

---

## ⚙️ Components List

### Core Modules
- **1x ESP32 MINI Kit** – Main MCU
- **2x KY-023 Joystick Module** – Dual-axis analog joystick + button
- **8x Push Buttons** – General input
- **4x Limit Switches** – Used as triggers
- **1x 18650 Battery Shield** – Dual-voltage output (3.3V and 5V)
- **2x 18650 Batteries** – Main power source
- **1x 0.66" OLED Display Shield** – Data/graphics display
- **1x NRF24L01+PA/LNA RF Module** – 2.4GHz wireless communication
- **1x Passive Buzzer** – PWM-based audio feedback
- **1x 2S Battery Indicator** – For battery level visibility

### Support Components
- **Capacitors:**
  - 1x 2200µF 16V (bulk smoothing for ESP32)
  - 1x 220µF 16V (decoupling for RF module)
  - Multiple 10nF / 100nF / 22pF (Tantalum and Ceramic for RF/ESP32 decoupling)
- **Diodes:**
  - Multiple 1N4148 (protection for RF module)
- **Resistors:**
  - Metal Film Resistors (1/4W): used for voltage division, pull-up/pull-downs, and resistor ladder matrix

---

## 🔌 Circuit Design

### Joystick Module Input Handling
The **KY-023** joystick operates at 5V but the ESP32 runs on 3.3V. Each joystick has **3 outputs** (VRX, VRY, SW). Instead of level-shifting with dividers on the power line, each signal output is protected using resistors (and optionally a diode):

```

[+5V]           [GND]
  |               |  
  |              [1k]
  |               |  
  +---------------+  
  |               |  
  |               |  
  |KY-023 Joystick|  
  |               |  
  |               |  
  ++------+------++  
   |      |      |   
 [VRX]  [VRY]   [SW] 
   |      |      |   
  [2k]   [2k]   [2k] 
   |      |      |   
  [D]    [D]    [D]  -> Optional Diode
   |      |      |
 [GPIO] [GPIO] [GPIO]

```

This setup ensures signal compatibility and adds protection, while maintaining full functionality.

---

### Button Matrix via Resistor Ladder
Due to limited GPIOs, **buttons are grouped** into two analog-input ladders (4 buttons per thumb). Ergonomically, this makes sense as thumbs typically activate one button at a time.

```

     [3.3V]
       |
       +---[Button 1]---[1k]---+
       |                       |
       +---[Button 2]---[2.2k]-+
       |                       |
       +---[Button 3]---[3.3k]-+----> [Analog Input Pin]
       |                       |
       +---[Button 4]---[4.7k]-+
                               |
                             [10k]
                               |
                              GND

```

Each press results in a distinct analog voltage that can be easily distinguished by the ESP32’s ADC.

---

### Other Connections
- **Limit Switches:** Connected directly to digital GPIOs for simultaneous multi-input detection
- **OLED Display:** I2C interface (ESP32 SDA/SCL)
- **NRF24L01+ Module:** SPI interface, additional decoupling for power stability
- **Buzzer:** Connected to a PWM-capable pin
- **Battery Indicator:** Connected directly to the battery terminals for independent monitoring

---

## 🧠 Ergonomics Considerations

This joystick project is designed around **personal hand comfort and control efficiency**, especially for a protoboard implementation. Key ergonomic decisions include:

### ➤ **Joystick–Joystick over Button–Button Configuration**
- Joysticks are placed **above the buttons**, giving them prominence and full thumb range.
- This **mirrored dual-stick layout** is designed for potential future RC applications, like tank-style or drone control.

### ➤ **Linear Button Layout**
- Buttons are arranged **linearly**, rather than in a traditional D-pad/cross format.
- This reduces **accidental presses** and aligns better with **natural thumb motion arcs**.
- Each side (left/right thumb) handles 4 buttons via resistor ladders.

### ➤ **Thumb and Finger Reach**
- **Joysticks are farther forward**, allowing thumb flexibility and reducing cramping during rotational input.
- **Trigger switches** are individually wired due to the possibility of **simultaneous index finger use**.

These choices are all made with comfort and responsiveness in mind, rather than blindly following industry layout norms.

---

## 🧪 Project Status

**Stage:** 🟡 Planning & Initial Hardware Testing  
This project will evolve in stages:

1. ✅ Schematic & component planning  
2. 🛠️ Hardware prototyping on protoboard  
3. 🔌 GPIO mapping and analog calibration  
4. 📡 Wireless comms via NRF24L01+  
5. 🧠 Firmware logic and state handling  
6. 🔋 Power and battery management  
7. 🧪 Usability testing and ergonomics tuning  
8. 🖼️ Optional: PCB design or 3D printed shell

---

## 📢 Author

**stndnt-c1**  
GitHub: [https://github.com/stdnt-c1](https://github.com/stdnt-c1)  
Project version: **v1.0.1-rev0**  
Date: **06-05-2025**

[Abstract Planner](plan/JoyStickPlan.ino)
[Pin Mapping](plan/PinPlanning.md)
---
