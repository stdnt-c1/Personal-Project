# Actual Implementation Approach

<img src="https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/back-view.jpg" alt="Joystick Core Prototype - BACK" width="35%" height="35%">
<img src="https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/front-view.jpg" alt="Joystick Core Prototype - FORNT" width="35%" height="35%">

- From the given [back view](https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/back-view.jpg), It's an prototype i've made on the protoboard, it has the **NRF24L01+PA/LNA** module along with the 220uF 16V capacitor in between the 3.3V and GND pin. <br>
- The **ESP32 Mini Kit** pins also made into a breakout with 2.200uF 16V capacitor between the 3.3V and GND pin as an additonal bulk capacitor. <br>
- There also an **Passive Buzzer** soldered on the board. <br>
- An additional **Switch** was added on the board as shown in the [front view](https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/front-view.jpg) to switch between _Program_ or _Live_ mode, in means to isolate the control and creates  safe environment. <br>
- The **0.66-inch White OLED Shield** also been already added to it, but in current implmentation, i'm using it upside-down.

# Pin Breakout
<img src="https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/wiring.jpg" alt="Core Breakout" width="35%" height="35%">

Based on the above [picture](https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/wiring.jpg), i'll list down the **ESP32 Mini Kit** on protoboard pinouts in detail. <br>

> __Note:__ "col" in here represents _column_ as viewed on the [picture](https://github.com/stdnt-c1/Personal-Project/blob/main/images/prototype/back-view.jpg), _col.1_ is the pinout near the center closer to central capacitor, while _col.2_ is the pinout farther away from the central capacitor. <br>
> __Note:__ Assuming if viewed from the back side with the USB port at the bottom, similar to [ESP32 back pinout](https://github.com/stdnt-c1/Personal-Project/blob/main/images/miniKit-pinout.png).

## Right Side
| RIGHT col.1 | RIGHT col.2 |
|-------------|-------------|
| RST         | SD3         |
| SVP         | TCK         |
| IO26        | GND         |
| IO18        | NC          |
| IO19        | SVN         |
| IO23        | IO35        |
| IO5         | IO33        |
| 3.3V        | IO34        |
|             | TMS         |
|             | NC          |
|             | SD2         |
|             | CMD         |

## Left Side
| LEFT col.1  | LEFT col.2  |
|-------------|-------------|
| TXD         | SD0         |
| RXD         | TD0         |
| IO22        | GND         |
| IO21        | IO27        |
| IO17        | IO25        |
| IO16        | IO32        |
| GND         | TDI         |
| VCC         | IO4         |
|             | IO0         |
|             | IO2         |
|             | SD1         |
|             | CLK         |
