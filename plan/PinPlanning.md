# ESP32 Mini Kit has these following pin

<img src="https://github.com/stdnt-c1/Personal-Project/blob/main/images/miniKit-pinout.png" alt="ESP32 Mini Kit" width="35%" height="35%">

## Inner Side:

__Note:__ Assuming viewed from below with the USB side pointing downwards.

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

__Note:__ Refer to [image](https://github.com/stdnt-c1/Personal-Project/blob/main/images/miniKit-pinout.png). 
__Note:__ The highlighted pin are adapted `D1 MINI` default pinout to ensure compatbility with their shields. 

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

__Note:__ Refer to [image](https://github.com/stdnt-c1/Personal-Project/blob/main/images/miniKit-pinout.png). 

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

## Pin Mapping

| Component         | Pin Functions          | ESP32 GPIO | Mini Kit GPIO | Type                      | Notes                                                                               |
| :---------------- | :--------------------- | :--------- | :------------ | :------------------------ | :---------------------------------------------------------------------------------- |
| **OLED Display** | SDA                    | **GPIO21** | IO21          | I2C                       | The OLED display is a shield. Connect to these I2C pins.                            |
|                   | SCL                    | **GPIO22** | IO22          | I2C                       |                                                                                     |
| **NRF24L01+PA/LNA** | MOSI                   | **GPIO23** | IO23          | SPI                       | HSPI MOSI                                                                           |
|                   | MISO                   | **GPIO19** | IO19          | SPI                       | HSPI MISO                                                                           |
|                   | SCK                    | **GPIO18** | IO18          | SPI                       | HSPI SCK                                                                            |
|                   | CSN                    | **GPIO17** | IO17          | Digital                   |                                                                                     |
|                   | CE                     | **GPIO16** | IO16          | Digital                   |                                                                                     |
| **Joystick 1 (Left)** | VR\_X                  | **GPIO36** | SVP           | ADC                       | Input-only. *Requires 5V to 3.3V voltage divider.* |
|                   | VR\_Y                  | **GPIO39** | SVN           | ADC                       | Input-only. *Requires 5V to 3.3V voltage divider.* |
|                   | SW (Button)            | **GPIO27** | IO27          | Digital (Pull-up to GND)  | Use internal pull-up. Safe GPIO on outer left side.                                |
| **Joystick 2 (Right)** | VR\_X                  | **GPIO34** | IO34          | ADC                       | Input-only. *Requires 5V to 3.3V voltage divider.* |
|                   | VR\_Y                  | **GPIO35** | IO35          | ADC                       | Input-only. *Requires 5V to 3.3V voltage divider.* |
|                   | SW (Button)            | **GPIO26** | IO26          | Digital (Pull-up to GND)  | Use internal pull-up.                                                               |
| **Buttons (Ladder 1)** | Analog Input           | **GPIO32** | IO32          | ADC                       | 4 buttons via resistor ladder. Use internal pull-up.                                |
| **Buttons (Ladder 2)** | Analog Input           | **GPIO33** | IO33          | ADC                       | 4 buttons via resistor ladder. Use internal pull-up.                                |
| **Trigger 1** | Digital Input          | **GPIO0** | IO0           | **NO** (Pull-up to GND)   | Strapping pin. Stays HIGH at boot. Connect to the **NO** side of the trigger.       |
| **Trigger 2** | Digital Input          | **GPIO2** | IO2           | **NO** (Pull-up to GND)   | Strapping pin. Stays HIGH at boot. Connect to the **NO** side of the trigger.       |
| **Trigger 3** | Digital Input          | **GPIO4** | IO4           | **NO** (Pull-up to GND)   | Safe GPIO for digital input. Connect to the **NO** side of the trigger.             |
| **Trigger 4** | Digital Input          | **GPIO5** | IO5           | **NC** (Pull-up to GND)   | Safe GPIO. Can be configured for NC connection. Available on inner right side.      |
| **Passive Buzzer** | Digital Output (PWM)   | **GPIO25** | IO25          | Digital (Output)          | Safe general-purpose GPIO for PWM.                                                  |

**Explanation of "Mini Kit GPIO" Column:**

* For pins like `GPIO21`, `GPIO22`, `GPIO17`, `GPIO16`, etc., they are directly labeled as `IO21`, `IO22`, `IO17`, `IO16` on the inner/outer sides of Mini Kit.
* `GPIO36` is labeled `SVP` on inner side.
* `GPIO39` is labeled `SVN` on outer side.
* `GPIO34` is labeled `IO34` on outer side.
* `GPIO35` is labeled `IO35` on outer side.
* `GPIO32` is labeled `IO32` on outer side.
* `GPIO33` is labeled `IO33` on outer side.
* `GPIO0` is labeled `IO0` on outer side.
* `GPIO2` is labeled `IO2` on outer side.
* `GPIO4` is labeled `IO4` on outer side.
* `GPIO25` is labeled `IO25` on outer side.
* `GPIO27` is labeled `IO27` on outer side.
* `GPIO5` is labeled `IO5` on inner side.

## Available Pins for Future Use

These pins are still available on your Mini Kit for additional components:

| ESP32 GPIO | Mini Kit Label | Location | Pin Type | Recommended Use | Notes |
|:-----------|:---------------|:---------|:---------|:----------------|:------|
| **GPIO15** | **TD0** | Inner Left | Digital I/O | General purpose, PWM, ADC2_CH3 | Safe for most applications |
| **GPIO13** | **TCK** | Inner Right | Digital I/O | General purpose, PWM, ADC2_CH4 | Safe for most applications |
| **GPIO14** | **TMS** | Outer Right | Digital I/O | General purpose, PWM, ADC2_CH6 | Safe, avoid if using JTAG debugging |

### ⚠️ Pins to Avoid:

| ESP32 GPIO | Mini Kit Label | Location | Why to Avoid |
|:-----------|:---------------|:---------|:-------------|
| **GPIO12** | **TDI** | Outer Left | **Strapping pin** - affects boot voltage. Can cause boot failures if pulled HIGH at startup |
| **GPIO6-11** | **CLK, SD0-3, CMD** | Various | **Connected to internal flash** - using these will brick your ESP32 |
| **GPIO1** | **TXD** | Inner Left | **UART TX** - needed for serial communication/debugging |
| **GPIO3** | **RXD** | Inner Left | **UART RX** - needed for serial communication/debugging |

## Pin Usage Summary

### Currently Used Pins:
- **Inner Side**: IO21 (SDA), IO22 (SCL), IO17 (CSN), IO16 (CE), IO26 (Joy2 SW), IO5 (Trigger4)
- **Outer Side**: SVP (Joy1 X), SVN (Joy1 Y), IO34 (Joy2 X), IO35 (Joy2 Y), IO32 (Buttons1), IO33 (Buttons2), IO0 (Trigger1), IO2 (Trigger2), IO4 (Trigger3), IO25 (Buzzer), IO27 (Joy1 SW)
- **SPI Pins**: IO23 (MOSI), IO19 (MISO), IO18 (SCK)

### Available for Expansion:
- **TD0 (GPIO15)** - Inner Left
- **TCK (GPIO13)** - Inner Right  
- **TMS (GPIO14)** - Outer Right

### Total Pin Count:
- **Used**: 16 pins
- **Available**: 3 pins
- **Avoid**: 7 pins (strapping/flash/UART)

__Note:__ For the missing/unsigned Mini Kit GPIO, you can use any generally safe pins from the available list above.
__Note:__ Refer to [image](https://github.com/stdnt-c1/Personal-Project/blob/main/images/miniKit-pinout.png).