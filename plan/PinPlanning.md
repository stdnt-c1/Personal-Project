# ESP32 Mini Kit Pin Planning - Updated (Post-LOG-6)

<img src="https://github.com/stdnt-c1/Personal-Project/blob/main/images/miniKit-pinout.png" alt="ESP32 Mini Kit" width="35%" height="35%">

## Inner Side:

> __Note:__ Assuming viewed from below with the USB side pointing downwards.

|  LEFT  | RIGHT |
|--------|-------|
| **TXD** | **RST** |
| **RXD** | **SVP** |
| **IO22** | **IO26** |
| **IO21** | **IO18** |
| **IO17** | **IO19** |
| **IO16** | **IO23** |
| **GND** | **IO5** |
| **VCC** | **3.3V** |
| TD0 | TCK |
| SD0 | SD3 |

> __Note:__ Refer to [image](https://github.com/stdnt-c1/Personal-Project/blob/main/images/miniKit-pinout.png). <br>
> __Note:__ The highlighted pin are adapted `D1 MINI` default pinout to ensure compatibility with their shields. 

## Outer Side

__Note:__ Assuming viewed from below with the USB side pointing downwards.

| LEFT | RIGHT |
|------|-------|
| GND | GND |
| IO27 | NC |
| IO25 | SVN |
| IO32 | IO35 |
| TDI | IO33 |
| IO4 | IO34 |
| IO0 | TMS |
| IO2 | NC |
| SD1 | SD2 |
| CLK | CMD |

> __Note:__ Refer to [image](https://github.com/stdnt-c1/Personal-Project/blob/main/images/miniKit-pinout.png). 

## Pin Name Explanation

The ESP32 Mini Kit uses simplified labels for many GPIO pins. Here's what they actually correspond to:

| Mini Kit Label | Actual ESP32 GPIO | Pin Type | Location | Notes |
|:---------------|:------------------|:---------|:---------|:------|
| **SVP** | **GPIO36** | ADC1_CH0 | Inner Right | Sensor VP - Input only, no pull-up/pull-down |
| **SVN** | **GPIO39** | ADC1_CH3 | Outer Right | Sensor VN - Input only, no pull-up/pull-down |
| **TXD** | **GPIO1** | UART0_TXD | Inner Left | UART TX - Used for serial communication |
| **RXD** | **GPIO3** | UART0_RXD | Inner Left | UART RX - Used for serial communication |
| **TD0** | **GPIO15** | MTDO | Inner Left | JTAG Test Data Out |
| **TCK** | **GPIO13** | MTCK | Inner Right | JTAG Test Clock |
| **TDI** | **GPIO12** | MTDI | Outer Left | JTAG Test Data In - **CAUTION: Strapping pin!** |
| **TMS** | **GPIO14** | MTMS | Outer Right | JTAG Test Mode Select |
| **SD0** | **GPIO7** | SD_DATA0 | Inner Left | Flash/SD card interface |
| **SD1** | **GPIO8** | SD_DATA1 | Outer Left | Flash/SD card interface |
| **SD2** | **GPIO9** | SD_DATA2 | Outer Right | Flash/SD card interface |
| **SD3** | **GPIO10** | SD_DATA3 | Inner Right | Flash/SD card interface |
| **CLK** | **GPIO6** | SD_CLK | Outer Left | Flash/SD card clock |
| **CMD** | **GPIO11** | SD_CMD | Outer Right | Flash/SD card command |
| **RST** | **EN** | Reset | Inner Right | Hardware reset pin |

**⚠️ Important Notes:**
- **SD pins (GPIO6-11)** are connected to the internal flash memory. **DO NOT USE** these pins for external connections as it will brick your ESP32.
- **TXD/RXD (GPIO1/3)** are used for serial communication. Avoid using them unless you disable serial output.
- **TDI (GPIO12)** is a strapping pin that affects boot mode. Use with caution or avoid entirely.

## Current Pin Assignment (Post-LOG-6 Implementation)

| Component              | Function            | ESP32 GPIO | Mini Kit Label | Type                    | Notes                                      |
|:---------------------- |:------------------- |:---------- |:-------------- |:----------------------- |:------------------------------------------ |
| **Joystick 1 (Left)**  | VRX                 | **GPIO36** | **SVP**        | ADC1                    | Voltage divider required (0–5V → 0–3.3V)  |
|                        | VRY                 | **GPIO39** | **SVN**        | ADC1                    | Voltage divider required                   |
|                        | SW                  | **GPIO27** | **IO27**       | Digital (INPUT_PULLUP)  |                                            |
| **Joystick 2 (Right)** | VRX                 | **GPIO34** | **IO34**       | ADC1                    | Voltage divider required                   |
|                        | VRY                 | **GPIO35** | **IO35**       | ADC1                    | Voltage divider required                   |
|                        | SW                  | **GPIO26** | **IO26**       | Digital (INPUT_PULLUP)  |                                            |
| **Push Buttons**       | Button 1            | **GPIO32** | **IO32**       | Digital (INPUT_PULLUP)  | Individual GPIO (no ladder)                |
|                        | Button 2            | **GPIO33** | **IO33**       | Digital (INPUT_PULLUP)  | Individual GPIO (no ladder)                |
|                        | Button 3            | **GPIO0**  | **IO0**        | Digital (INPUT_PULLUP)  | Strapping pin – protected by MOSFET gating |
|                        | Button 4            | **GPIO2**  | **IO2**        | Digital (INPUT_PULLUP)  | Strapping pin – protected by MOSFET gating |
|                        | Button 5            | **GPIO4**  | **IO4**        | Digital (INPUT_PULLUP)  |                                            |
|                        | Button 6            | **GPIO5**  | **IO5**        | Digital (INPUT_PULLUP)  |                                            |
|                        | Button 7            | **GPIO15** | **TD0**        | Digital (INPUT_PULLUP)  | Newly utilized                             |
|                        | Button 8            | **GPIO13** | **TCK**        | Digital (INPUT_PULLUP)  | Newly utilized                             |
| **OLED Display**       | SDA                 | **GPIO21** | **IO21**       | I2C                     |                                            |
|                        | SCL                 | **GPIO22** | **IO22**       | I2C                     |                                            |
| **NRF24L01+PA/LNA**    | CE                  | **GPIO16** | **IO16**       | Digital                 |                                            |
|                        | CSN                 | **GPIO17** | **IO17**       | Digital                 |                                            |
|                        | MOSI                | **GPIO23** | **IO23**       | SPI                     |                                            |
|                        | MISO                | **GPIO19** | **IO19**       | SPI                     |                                            |
|                        | SCK                 | **GPIO18** | **IO18**       | SPI                     |                                            |
| **Buzzer**             | PWM Output          | **GPIO25** | **IO25**       | Digital (PWM)           |                                            |
| **Button Power Gate**  | MOSFET Gate Control | **GPIO14** | **TMS**        | Digital (Output)        | Controls IRF740 for button power isolation |

## Hardware Protection Implementation

### Button Power Gating (LOG-6 Innovation)
All button VCC lines are controlled through an **IRF740 N-channel MOSFET** to prevent boot interference:

```
VCC_IN ──┬─── IRF740 Drain ───→ VCC_OUT (All Button Power)
         │
         └─── Source ──→ GND
              │
              Gate ←── 10Ω Resistor ←── GPIO14 (TMS)
```

**Protection Logic:**
- **Boot Phase**: GPIO14 = LOW → MOSFET OFF → Buttons isolated
- **Runtime Phase**: GPIO14 = HIGH → MOSFET ON → Buttons powered

This eliminates strapping pin interference from GPIO0 and GPIO2 during ESP32 boot sequence.

### Voltage Dividers for Joystick Inputs
All joystick analog outputs (VRX/VRY) use **10kΩ + 20kΩ** voltage dividers:

```
Joystick_Output (0-5V)
       │
     [10kΩ]  ← R1
       │
     [GPIO]  ← ESP32 ADC Input (0-3.3V)
       │
     [20kΩ]  ← R2
       │
      GND
```

### Power Filtering
**ESP32 Bulk Capacitance:**
- 2200µF 16V Electrolytic + 100nF 50V Ceramic in parallel

**NRF24L01 Protection:**
- 2200µF 16V Electrolytic + 100nF 50V Ceramic in parallel
- Prevents RF module brownout and filters power noise

## Major Changes from Original Planning

### ❌ **Removed Features:**
- **Resistor Ladder Inputs**: Replaced with individual GPIO assignments for simultaneous button presses
- **Trigger Buttons**: Eliminated due to mechanical constraints  
- **Manual Program/Live Mode Switch**: Replaced with automatic MOSFET gating

### ✅ **Added Features:**  
- **Hardware Button Isolation**: IRF740 MOSFET prevents boot interference
- **Individual Button GPIOs**: All 8 buttons have dedicated pins
- **Enhanced Power Filtering**: Dual-capacitor setup for both ESP32 and NRF24L01
- **Automatic Boot Protection**: No user intervention required for safe operation

### 🔄 **Pin Reassignments:**
- **GPIO14**: Changed from unused to **Button Power Gate Control**
- **GPIO15 & GPIO13**: Activated for **Button 7 & 8**
- **GPIO32 & GPIO33**: Changed from **ADC Ladder** to **Individual Button** inputs

## Pin Usage Summary

### Currently Used Pins: **19/30 Available**
- **ADC Inputs (4)**: GPIO36, GPIO39, GPIO34, GPIO35 (Joystick axes)
- **Digital Inputs (10)**: GPIO27, GPIO26, GPIO32, GPIO33, GPIO0, GPIO2, GPIO4, GPIO5, GPIO15, GPIO13 (Buttons & Joystick switches)
- **Digital Outputs (2)**: GPIO25 (Buzzer), GPIO14 (Button Gate)
- **I2C (2)**: GPIO21 (SDA), GPIO22 (SCL)
- **SPI (3)**: GPIO18 (SCK), GPIO19 (MISO), GPIO23 (MOSI), GPIO16 (CE), GPIO17 (CSN)

### Available for Future Expansion: **0 Safe Pins**
All safe, general-purpose GPIO pins are now utilized. Future expansion would require:
- **I2C GPIO Expander** (PCF8574, MCP23017)
- **SPI Shift Registers** (74HC595 for outputs, 74HC165 for inputs)
- **Alternative communication pins** (if UART/JTAG debugging not needed)

### Pins Avoided: **7 Pins**
- **GPIO1, GPIO3**: UART TX/RX (Serial communication)
- **GPIO6-11**: Flash interface (DO NOT USE - will brick ESP32)
- **GPIO12**: Strapping pin (TDI - boot voltage selection)

## Bootup Safety Notes

**Strapping Pin Management:**
- **GPIO0**: Used for Button 3 – Safe due to MOSFET isolation during boot
- **GPIO2**: Used for Button 4 – Safe due to MOSFET isolation during boot  
- **GPIO12**: **AVOIDED** – Critical strapping pin for boot voltage
- **GPIO15**: Used for Button 7 – Safe (internal pull-up OK)

**Boot Sequence:**
1. ESP32 powers up with GPIO14 = LOW (default)
2. IRF740 gate LOW → All buttons disconnected
3. ESP32 completes boot without strapping pin interference  
4. Firmware sets GPIO14 = HIGH → Buttons become active
5. Normal operation begins

This implementation ensures **100% reliable boot** regardless of button states during power-on.