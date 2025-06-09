# Project Progress Log

## LOG-1 | 05/06/25 | Abstract Planning
* Starting out <br>
What's tthe fun in having a joystick but not constructing it yourself? DIY your way into creating it.
Here for i've added some approaches of how i will be starting this project, such as follows:
  + Listing components on hands
  + Creating abstract wiring
  + Mkaing some considerations

I've already constructing some possible shapes of the joystick while at it and plan the buttons.

## LOG-2 | 06/06/25 | Refining the Plan
* First prototype <br>
I noticed i've missed some issue regarding the KY-023 signal output handling, where i fed it 5v yet didn't apply the correct 3.3v voltage divider for it, so i fixed that and added some simple visualizations.
Along with that, i also experiment further the joystick, but it came out too bulky, and i was too lazy to pickup my power tools becuase it's late night already.
Some of the small components have arrived, so i guess i'll continue later.

## LOG-3 | 07/06/25 | Constructing the Base
* Initial structure <br>
While at it, i choose to map the pinout and review again the approach, and i do realizes that some of my input might interfere with the bootup of the ESP32.
I've made a modifications where for some specific pins, i connect it to the limit/micro switch to NO to get the pin HIGH at boot to avoid issue.
Since i choose NRF24L01 as the primary communication, i will disable WiFi and BLE features on the ESP32 in case of interference on some of the pins.
Now, i also attempted to construct the joystick by making some holes for the M3 PCB spacers and creates a custom layout, it came out nice.
But i've done something wrong, i don't know how to place the trigger pins to be comfortably usable, i'll think about that later.
Due to protoboard and buttons size constraints, i choose to make the dominant side to have the linear button layout while the other side have the D-pad/cross button layout.
Next i might started soldering them since the structure and layout already in place.

## LOG-4 | 08/06/25 | Resistive Divider and Revision
* Joystick diagram and wiring revision (rev-1) <br>
Previously, our joystick diagram are as follows:
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

But it seems to be wrong because the output to GPIO weren't isolated, the following diagram are the simplified visualizations of our current divider:
```
[+5V]----+
         |
       [VRX]
         |
       [2kΩ]
         |
       [GPIO] ← BAD: Without a lower leg to GND, this is not a proper divider
         |
       [1kΩ]
         |
       GND
```

And the revised one are as follows:
```
Joystick Output (e.g., VRX)
       |
      [10kΩ]  ← R1
       |
     [GPIO]   ← ESP32 analog input
       |
      [20kΩ]  ← R2
       |
      GND
```

In assumptions is that the Joystick will output direct 0-5v signal, but the signal are fed back to the Joystick itself, meaning improper divider. In short, we revised it to be as follows:
```
[+5V]---+ +---[GND] ← Joystick VCC adn GND
        | |
       [VRX]
         |
       [10kΩ] ← 2k to 10k
         |
       [GPIO] 
         |
       [20kΩ] ← 1k to 20k
         |
        GND ← ESP GND
```

This is the similar case of implementation with the LDR sensor where we instead read the value in between instead of straight,
```
  [LDR_SENSOR]
    |      |
    |      |
    |     VCC
    |
    +--- GPIO
    |
  [10k]
    |
   GND
```
> __NOTE:__ The above diagram is an LDR Sensor wiring example for **Arduino Uno R3** and mentioned only as a reference.

Isolating the power to route back to ESP and not to Joystick GND, the flow as follows:
```
  JOYSTICK------------------+
     |                      |
SIGNAL_OUTPUT               |
     |                      |
     +-----------+          |
     |           |          |
  ESP_GPIO    ESP_GND    ESP_GND
```

This because previously we tried to do voltage divider by reusing the Joystick GND for the Divider GND, where in practice, all grounds should be common to themself. But it doesn't mean we were required to use individual GND for this purpose, the only issue is that we were just routing the power back to the joystick instead of the ESP common, which results in improper voltage divider.

In essence, instead of this:
```
   JOYSTICK------+
      |          |
      | ← VCC    | ← GND
      |          |
     ESP---------+
```
Where connection are flowing through the same line, we need this:
```
   JOYSTICK------+
      |          |
      | ← VCC    | 
      +----+     |
      |    |     |
     ESP  GND1  GND2
```
Where the line is dedicated, but it doesn't mean you need separate common ground for them. You can wire _GND1_ and _GND2_ together as the common base, but **don't** make it flow through the module.

* NRF24L01+PA/LNA protection plan <br>
I did mentioned that we have **100nF Ceramic Capacitor**, so we'll use that to as a filter for our RF modules, currently we have the RF module wired as follows:
```
  NRF24L01_VCC----+
                  |
                  |
            [2200uF 16V]
                  |
                  |
  NRF24L01_GND----+
```

But that's not enough, considering RF modules, especially NRF24L01 operates at 3.3v and slight interference would and could brick it and power draw peaked at high power, having bulk capacitor alone is not enough since even though it offers additonal power during peak operations, it's still has the possibilty to carry electrical noise, so we need ceramic capacitor which significantly has lower ESR to filter out low noise. Now the diagram looks like this:
```
  NRF24L01_VCC----+-----------+
                  |           |
                  |           |
            [2200uF 16V]   [100nF 50V]
                  |           |
                  |           |
  NRF24L01_GND----+-----------+
```

By wiring both **Electrolytic Capacitor** and **Ceramic Capacitor** in parallel with the power input to ground, we essentially creates an additional power bank and robust filter for both HIGH noise and LOW noise going into the NRF24L01 module.

* ESP32 central capacitor filter <br>
Current ESP32 state is as shown in the [picture](https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/wiring.jpg), and if our RF module has extra ceramic capacitor, so why wouldn't we apply the same to the bulk capacitor on our ESP32? And so i did add another **100nF 50V** in parallel, the same wiring as the RF module.

## LOG-5 | 09/06/25 | Input Exclusions
* Input Exclusions <br>
Been prototyping for a while, and now i realizes that i can't put the trigger anywhere, so i choose to opt-out the trigger function. Instead, we'll refine the available input. Currently i've contructed the base as follows:

> Joystick from above
<img src="https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/joystick-above.jpg" alt="Joystick Core Prototype - BACK" width="35%" height="35%">

> Joystick from below
<img src="https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/joystick-below.jpg" alt="Joystick Core Prototype - BACK" width="35%" height="35%">

> Joystick from side
<img src="https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/joystick-side.jpg" alt="Joystick Core Prototype - BACK" width="35%" height="35%">

* Input Refinement <br>
Following the modifications, i also wanted to refine the button resistor-ladder layout, since we've opted out 4 trigger buttons, it means we also gained 4 extra GPIO. Utilizing the gained GPIO pins available, we will cancel the ladder pin and instead wired individually each buttons to the GPIO, allowing simulatenous input from all the buttons for comforts.

* Mode Switch <br>
As you can see from the [front view](https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/front-view.jpg) of our joystick core near the buzzer, there's a **switch** that we planned to be used as switch mode between _Program_ and _Live Mode_, where the function are as follows:
  + _Program Mode_ <br>
  If switch are set to _top_ (program), it requires the ESP32 to be reloaded/rebooted manually by users through onboard **RST** button to apply the mode switching. Upon mode switch, the users can't use any of the inputs, and is required to wire onto a computer for serial debugging, this also allows for granular control when attempting modifications and uploads/testing safely.

  + _Live Mode_ <br>
  If switch are set to _bottom_ (live), it requires the ESP32 to be realoded/rebooted mmanually by users through onboard **RST** button to apply the mode switching. Upon mode switch, the serial is blocked and boot are in normal operations, blocking any potential _special_ operations by ESP32 for smoother operations during usage.

* Revised GPIO Pin Assignment (Post-LOG-5)
Following the design decisions, we have revised the pin planning from the existing [planner](https://github.com/stdnt-c1/Personal-Project/blob/main/plan/PinPlanning.md) into the following configurations:

| Component              | Function            | ESP32 GPIO | Mini Kit Label | Type                    | Notes                              |
| ---------------------- | ------------------- | ---------- | -------------- | ----------------------- | ---------------------------------- |
| **Joystick 1 (Left)**  | VRX                 | GPIO36     | SVP            | ADC1                    | Via voltage divider                |
|                        | VRY                 | GPIO39     | SVN            | ADC1                    | Via voltage divider                |
|                        | SW (Button)         | GPIO27     | IO27           | Digital (INPUT\_PULLUP) |                                    |
| **Joystick 2 (Right)** | VRX                 | GPIO34     | IO34           | ADC1                    | Via voltage divider                |
|                        | VRY                 | GPIO35     | IO35           | ADC1                    | Via voltage divider                |
|                        | SW (Button)         | GPIO26     | IO26           | Digital (INPUT\_PULLUP) |                                    |
| **Push Buttons**       | Button 1            | GPIO32     | IO32           | Digital (INPUT\_PULLUP) | Formerly resistor ladder 1         |
|                        | Button 2            | GPIO33     | IO33           | Digital (INPUT\_PULLUP) | Formerly resistor ladder 2         |
|                        | Button 3            | GPIO0      | IO0            | Digital (INPUT\_PULLUP) | Safe if pulled HIGH at boot        |
|                        | Button 4            | GPIO2      | IO2            | Digital (INPUT\_PULLUP) | Safe if pulled HIGH at boot        |
|                        | Button 5            | GPIO4      | IO4            | Digital (INPUT\_PULLUP) |                                    |
|                        | Button 6            | GPIO5      | IO5            | Digital (INPUT\_PULLUP) |                                    |
|                        | Button 7            | GPIO15     | TD0            | Digital (INPUT\_PULLUP) | Newly added from available pool    |
|                        | Button 8            | GPIO13     | TCK            | Digital (INPUT\_PULLUP) | Newly added from available pool    |
| **OLED Display**       | SDA                 | GPIO21     | IO21           | I2C                     |                                    |
|                        | SCL                 | GPIO22     | IO22           | I2C                     |                                    |
| **NRF24L01+PA/LNA**    | CE                  | GPIO16     | IO16           | Digital                 |                                    |
|                        | CSN                 | GPIO17     | IO17           | Digital                 |                                    |
|                        | MOSI                | GPIO23     | IO23           | SPI                     |                                    |
|                        | MISO                | GPIO19     | IO19           | SPI                     |                                    |
|                        | SCK                 | GPIO18     | IO18           | SPI                     |                                    |
| **Buzzer**             | PWM Output          | GPIO25     | IO25           | PWM Digital Output      |                                    |
| **Mode Switch**        | Program/Live toggle | GPIO14     | TMS            | Digital (INPUT\_PULLUP) | Requires manual reset after change |

With this, we've used up all of our **Safe General-purpose GPIO** pins available for the project, and any further add-ons or implementation to be physically added should involve **I2C IO Expander** or **Shift Register** to control the extra I/O. For this project, it's enough of them. What's left now os to review again the planning and start the actual implementation as per our considerations and modification, but for today, i will stop here since i need to gather the requirements first.

## LOG-6 | 10/06/25 | Bootup Gate Protection
* Replaced Program/Live Switch with Automatic Button Isolation <br>
During testing, I realized the manual **Program/Live Mode switch** was redundant and added user friction. Additionally, several of my button inputs are connected to **strapping-sensitive GPIOs** like `GPIO0` and `GPIO2`, which can **interfere with ESP32 boot behavior** if pulled LOW during startup.

To resolve both concerns, I redesigned the button input system using a **hardware gating approach**:

* All **button VCC lines** are routed through an **N-channel MOSFET**.
* The **MOSFET gate** is controlled by the ESP32 using **GPIO14**.
* On boot, the gate remains LOW, **keeping the buttons disconnected** from power.
* After boot, the firmware sets GPIO14 HIGH, **enabling all buttons simultaneously**.

This eliminates startup issues and makes the boot process **clean and automatic**, without requiring users to flip any switches.

```cpp
// Safe power gating for all buttons post-boot
#define BTN_POWER_GATE 14
void setup() {
  pinMode(BTN_POWER_GATE, OUTPUT);
  digitalWrite(BTN_POWER_GATE, LOW);  // Keep buttons off
  delay(500);                          // Allow boot process to complete
  digitalWrite(BTN_POWER_GATE, HIGH); // Enable all buttons
}
```
> __NOTE:__ The above is a simple example of the protection, use a proper method for the safe activation.

This change freed up the physical switch previously used for mode toggling and streamlined the wiring for better long-term maintenance.

* MOSFETs Choice and Wiring <br>
While **IRF740** is a popular, widely available MOSFET in the market, the gate threshold for them are quite high, around **2V-4V**, even though our ESP32 GPIO can drive, i would like to have a smaller ones, for that, i've listed some common choices as follows:
  + **2N7000** *0.8V~3V | Current Cap. ~200mA continuous, ~800mA pulsed*
  + **BS170** *0.8V~3V | Current Cap. ~500mA continous, ~1200mA pulsed*
  + **IRLZ44N** *1V~2V | Current Cap. ~47A*

Based on that list, we'll use **2N7000** since it's more fitting, you can use the other or even uses the previous IRF740 MOSFET for this, but i'll choose 2N7000 for this one, you could even uses IRLZ44N, though 47A is rather overkill for this project, but not prohibited anyway, BS170 guarantee you a logic level activation since their threhold are very low.

  + MOSFET Wiring <br>
  Some of you may be confused of how to determine the MOSFET wiring, here's some basic example for you and the wiring for the project:

    - **TO-92 Package (_Half-circle shape_)** <br>
    Standard orientation to determine the pinout for this kind of MOSFET is with the **flat side facing you** and the **leads/pin facing down**.

    Examples:

      - **2N7000 (Most Common)** <br>
      ```
       [ Curved Side ]
        S     G     D
        |     |     |
      Source-Gate-Drain
        [ Flat Side]
      ```
      > __NOTE:__ Iteration from **Left-to-Right**

      - **BS170 (Traditional)** <br>
      ```
       [ Curved Side ]
        D     G     S
        |     |     |
      Drain-Gate-Source
        [ Flat Side]
      ```
      > __NOTE:__ Iteration from **Left-to-Right** <br>
      > __NOTE:__ There's also new kind of **BS170** where some newer model might have the **S-G-D** sequence instead of **D-G-S** sequence.

    - **TO-220 Package (_Squeare/Rectangular shape_)** <br>
    Standard orientation to determine the pinout for this kind of MOSFET is with the MOSFET **facing towards you** (where you can read the part number) and the **metal tab/heatsink facing away from you** (facing the floor).

    Examples:
      
      - **IRF740 (Most Common)** <br>
      ```
     [Metal tab/heatsink at back]
        _________________
       |                 |
       |     IRF740      |  <- You read this side
       |_________________|
            |   |   |
            1   2   3
            |   |   |
        Gate Drain Source
      ```
      > __NOTE:__ For IRF740 specifically: **G-D-S** (Gate-Drain-Source) from left to right.

  * Project wiring <br>
  And so, for the project, we simply just need to wire it like this:
  ```
     +------[BATTERY 3.3V]
     |
    [D]
     |
     |
  [2N7000]---[G]--[10k]---[ESP_GPIO]
     |                                    
     |                                    
    [S]                                   
     |                                    
     +-------------[BUTTONS_VCC]----[GND]
  ```
  > __NOTE:__ **D** stands for **Drain**, **G** stands for **Gate**, **S** stands for **Source**.

* Revised GPIO Pin Assignment
  
| Component              | Function            | ESP32 GPIO | Mini Kit Label | Type                    | Notes                                      |
| ---------------------- | ------------------- | ---------- | -------------- | ----------------------- | ------------------------------------------ |
| **Joystick 1 (Left)**  | VRX                 | GPIO36     | SVP            | ADC1                    | Voltage divider required (0–5V → 0–3.3V)   |
|                        | VRY                 | GPIO39     | SVN            | ADC1                    | Voltage divider required                   |
|                        | SW                  | GPIO27     | IO27           | Digital (INPUT\_PULLUP) |                                            |
| **Joystick 2 (Right)** | VRX                 | GPIO34     | IO34           | ADC1                    | Voltage divider required                   |
|                        | VRY                 | GPIO35     | IO35           | ADC1                    | Voltage divider required                   |
|                        | SW                  | GPIO26     | IO26           | Digital (INPUT\_PULLUP) |                                            |
| **Push Buttons**       | Button 1            | GPIO32     | IO32           | Digital (INPUT\_PULLUP) | Formerly ladder input                      |
|                        | Button 2            | GPIO33     | IO33           | Digital (INPUT\_PULLUP) | Formerly ladder input                      |
|                        | Button 3            | GPIO0      | IO0            | Digital (INPUT\_PULLUP) | Strapping pin – protected by MOSFET gating |
|                        | Button 4            | GPIO2      | IO2            | Digital (INPUT\_PULLUP) | Strapping pin – protected by MOSFET gating |
|                        | Button 5            | GPIO4      | IO4            | Digital (INPUT\_PULLUP) |                                            |
|                        | Button 6            | GPIO5      | IO5            | Digital (INPUT\_PULLUP) |                                            |
|                        | Button 7            | GPIO15     | TD0            | Digital (INPUT\_PULLUP) | Newly used                                 |
|                        | Button 8            | GPIO13     | TCK            | Digital (INPUT\_PULLUP) | Newly used                                 |
| **OLED Display**       | SDA                 | GPIO21     | IO21           | I2C                     |                                            |
|                        | SCL                 | GPIO22     | IO22           | I2C                     |                                            |
| **NRF24L01+PA/LNA**    | CE                  | GPIO16     | IO16           | Digital                 |                                            |
|                        | CSN                 | GPIO17     | IO17           | Digital                 |                                            |
|                        | MOSI                | GPIO23     | IO23           | SPI                     |                                            |
|                        | MISO                | GPIO19     | IO19           | SPI                     |                                            |
|                        | SCK                 | GPIO18     | IO18           | SPI                     |                                            |
| **Buzzer**             | PWM Output          | GPIO25     | IO25           | Digital (PWM)           |                                            |
| **Button Power Gate**  | IRF740 Gate Control | GPIO14     | TMS            | Digital (Output)        | Enables button power after boot            |
