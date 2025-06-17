# ESP32 Dual Joystick Wireless Controller  
![Version](https://img.shields.io/badge/version-v1.0.3-blue)  
![Status](https://img.shields.io/badge/status-Hardware_Complete-green)  
![License](https://img.shields.io/badge/license-MIT-green)  
![ESP32](https://img.shields.io/badge/platform-ESP32-blueviolet)  
![DIY](https://img.shields.io/badge/type-Hardware--Project-informational)

---

## 🎮 Project Overview

A **custom-built ESP32-based dual joystick wireless controller** designed from scratch for gaming, robotics control, and wireless input experimentation. This project showcases advanced hardware prototyping with careful consideration for power management, signal integrity, and ergonomic design.

### Key Features
- **Dual analog joysticks** with push-button functionality
- **8 individual digital buttons** with simultaneous input capability
- **Wireless NRF24L01+ communication** for reliable data transmission  
- **OLED display** for real-time status and feedback
- **Smart power management** with boot-safe button isolation
- **Ergonomic perfboard design** optimized for comfortable gameplay
- **Robust power filtering** for stable RF operation

---

## 🚀 What Makes This Special

### Advanced Hardware Solutions
- **🔒 Boot Protection**: IRF740 MOSFET gating prevents GPIO conflicts during ESP32 startup
- **⚡ Power Conditioning**: Multi-stage filtering (2200µF + 100nF) for clean RF operation  
- **📏 Voltage Scaling**: Proper 5V→3.3V dividers for joystick analog inputs
- **🎯 Individual GPIO Mapping**: Every button gets its own pin for simultaneous inputs

### Smart Design Decisions
- **Eliminated resistor ladders** in favor of direct GPIO mapping
- **Strategic pin selection** avoiding ESP32 boot-strapping conflicts
- **Optimized ergonomics** with joysticks positioned above buttons
- **Linear button layout** for natural thumb movement patterns

---

## ⚙️ Hardware Architecture

### Core Components
| Component | Purpose | Key Specs |
|-----------|---------|-----------|
| **ESP32 MINI Kit** | Main MCU | 240MHz dual-core, WiFi/BLE |
| **2× KY-023 Joysticks** | Analog input | Dual-axis + push-button |
| **8× Push Buttons** | Digital inputs | Individual GPIO mapping |
| **IRF740 MOSFET** | Boot protection | VCC gating for safe startup |
| **NRF24L01+PA/LNA** | Wireless comm | Long-range RF transceiver |
| **0.66" OLED Display** | Status feedback | I²C interface |
| **Passive Buzzer** | Audio alerts | PWM-driven |
| **Dual 18650 + Shield** | Power system | 5V/3.3V regulated output |

### Power & Signal Conditioning
- **Multi-stage filtering**: 2200µF electrolytic + 100nF ceramic capacitors
- **ESD protection**: 1N4148 diodes on sensitive inputs  
- **Voltage dividers**: 10kΩ/20kΩ for 5V→3.3V joystick scaling
- **Pull-up networks**: Proper input conditioning for all digital pins

---

## 🔌 GPIO Pin Mapping

| Function | ESP32 GPIO | Type | Notes |
|----------|------------|------|-------|
| **Left Joystick** | | | |
| ├─ VRX | GPIO36 (SVP) | ADC1 | Via voltage divider |
| ├─ VRY | GPIO39 (SVN) | ADC1 | Via voltage divider |
| └─ SW | GPIO27 | Digital | Pull-up enabled |
| **Right Joystick** | | | |
| ├─ VRX | GPIO34 | ADC1 | Via voltage divider |
| ├─ VRY | GPIO35 | ADC1 | Via voltage divider |
| └─ SW | GPIO26 | Digital | Pull-up enabled |
| **Push Buttons** | | | |
| ├─ Button 1-2 | GPIO32, 33 | Digital | Former ladder inputs |
| ├─ Button 3-4 | GPIO0, 2 | Digital | **Boot-safe via MOSFET** |
| └─ Button 5-8 | GPIO4,5,15,13 | Digital | Standard inputs |
| **Peripherals** | | | |
| ├─ OLED (SDA/SCL) | GPIO21, 22 | I²C | Status display |
| ├─ NRF24 (CE/CSN) | GPIO16, 17 | Digital | RF control |
| ├─ NRF24 (SPI) | GPIO18,19,23 | SPI | Data interface |
| ├─ Buzzer | GPIO25 | PWM | Audio feedback |
| └─ **Button Gate** | GPIO14 | Output | **MOSFET control** |

---

## 🛡️ Boot Protection System

**Problem**: ESP32 GPIOs like GPIO0 and GPIO2 are boot-strapping pins - pulling them LOW during startup can prevent normal boot.

**Solution**: Hardware-gated button power using IRF740 MOSFET:

```cpp
#define BTN_POWER_GATE 14

void setup() {
  pinMode(BTN_POWER_GATE, OUTPUT);
  digitalWrite(BTN_POWER_GATE, LOW);  // Buttons OFF during boot
  delay(500);                          // Allow clean startup
  digitalWrite(BTN_POWER_GATE, HIGH); // Enable all buttons
}
```

This ensures **zero interference** with ESP32 boot sequence while maintaining full button functionality during operation.

---

## 🔧 Circuit Highlights

### Joystick Voltage Scaling
```
KY-023 Output (0-5V) ──[10kΩ]── GPIO ──[20kΩ]── GND
                                  ↑
                              (0-3.3V safe)
```

### RF Power Conditioning  
```
VCC ──┬──[2200µF]──┬── NRF24L01+
      └──[100nF]───┘
      ↑              ↑
   Bulk storage   Noise filter
```

### Button Protection
```
3.3V ──[IRF740]── Button VCC
         ↑
    GPIO14 control
```

---

## 📈 Development Timeline

| Phase | Status | Details |
|-------|--------|---------|
| ✅ **Design & Planning** | Complete | Component selection, pin mapping |
| ✅ **Hardware Prototyping** | Complete | Perfboard layout, component placement |
| ✅ **Power System Design** | Complete | Voltage regulation, filtering, protection |
| ✅ **Boot Protection** | Complete | MOSFET gating, safe startup sequence |
| ✅ **Physical Assembly** | Complete | Soldering, wiring, mechanical assembly |
| 🔄 **Firmware Development** | In Progress | NRF communication, input handling |
| 🔜 **UI Implementation** | Upcoming | OLED display, status indicators |
| 🔜 **Wireless Testing** | Upcoming | Range testing, latency optimization |

---

## 🎯 Design Philosophy

### Ergonomics First
- **Joysticks positioned above buttons** for natural thumb reach
- **Linear button arrangement** instead of cramped D-pad layouts  
- **Comfortable grip tested** with real-world usage scenarios
- **No triggers** - simplified design for better handling

### Electrical Integrity
- **Individual button GPIOs** eliminate input conflicts
- **Proper voltage scaling** protects ESP32 from 5V joystick signals
- **Multi-stage filtering** ensures clean power for sensitive RF circuits
- **Boot-safe design** prevents startup issues from user inputs

### Maintainable Architecture
- **Clear pin documentation** for easy modifications
- **Modular power domains** for safe testing and debugging
- **Standard components** for easy sourcing and replacement

---

## 📚 Documentation Structure

```
├── README.md                 # This overview
├── progress-log.md           # Detailed development journal  
├── plan/
│   ├── PinPlanning.md       # Complete GPIO assignments
│   └── JoyStickPlan.ino     # Initial firmware framework
└── logs/
    ├── LOG-7.md             # Physical assembly photos
    └── LOG-8.md             # Final wiring and testing
```

---

## 🔍 Key Learning Outcomes

This project demonstrates:
- **Advanced ESP32 GPIO management** with boot-strapping considerations
- **Mixed-signal design** combining analog joysticks with digital controls  
- **RF circuit design** with proper power conditioning and filtering
- **Mechanical prototyping** with ergonomic considerations
- **Power domain isolation** using MOSFET switching circuits

---

## 👨‍💻 Author

**stndnt-c1** | [GitHub Profile](https://github.com/stdnt-c1)

*"Why buy a controller when you can engineer a better one?"*

---

## 📄 License

This project is licensed under the **MIT License** - see the `LICENSE` file for details.

**Hardware designs, circuit schematics, and documentation are freely available for educational and personal use.**