/**
 * @date 05-06-2025
 * @version 1.0.0-beta.1 (rev0)
 * @author stndnt-c1 [https://github.com/stdnt-c1]
 * 
 * @file JoyStickPlan.ino
 * @brief This sketch demonstrates a plan of creating my own Joystick.
 * 
 * @details
 * This sketch is made to outline my planning into creating my own custom-made ESP32-based Joystick on Arduino.
 * It is not a complete implementation or by any means a working code.
 * It serves as a conceptual framework for the project. (Though work is in progress)
 * It only outlines the structure and the components that will be used in the project.
 * 
 * @note
 * This sketch is not intended to be run as-is. It is a plan and does not contain complete code.
 * It is meant to be a starting point for further development and implementation.
 * 
 * @details
 * The detailed components and their specifications are listed below:
 * - 1x ESP32 MINI Kit: This is the main microcontroller that will be used in the project.
 * - 2x KY-023 Joystick Module: This module will be used to read the joystick inputs.
 * - 8x Push Button: These buttons will be used for additional controls.
 * - 4x Limit Switch: These switches will act as trigger buttons for the joystick.
 * - 1x 18650 Battery Shield: This shield will provide 3.3v and 5v power simultaneously to the joystick.
 * - 2x 18650 Battery: These batteries will power the joystick.
 * - 1x 0.66-inch White OLED Display Shield: This display is an additonal component that will be used to show information on the joystick (and some cool graphics).
 * - 1x NRF24L01+PA/LNA RF Module: This modules is out primary communication module that will be used to communicate with the PC (and potentially future RC projects).
 * - 1x Passive Buzzer: This buzzer is just an additional cosmetics component that will be used to provide audio feedback for the joystick through PWM signal.
 * - 1x 2S Battery Indicator: This is an additional component that will be used to indicate the battery level of the joystick.
 * 
 * Some addional small components that may be used in the project:
 * - 1x 2200uF 16v Electrolytic Capacitor: This capacitor will be used as an additional bulk capacitor for the ESP32 itself.
 * - 1x 220uF 16v Electrolyctic Capacitor: This capacitor will be used as an smoothing capacitor for the RF module.
 * - anyX 10nF 50v Tantalum Capacitor (103): This capacitor will be used as an additional decoupling capacitor for the RF module.
 * - anyX 100nF 50v Tantalum Capacitor (104): This capacitor will be used as an additional decoupling capacitor for the RF module
 * - anyX 10nF 50v Tantalum Capacitor (103): This capacitor will be used as an additional decoupling capacitor for the RF module.
 * - anyX 22pF 50v Ceramic Capacitor (22): This capacitor will be used as an additional decoupling capacitor for the RF module.
 * - anyX 100nF 50v Ceramic Capacitor (104): This capacitor will be used as an additional decoupling capacitor for the RF module.
 * - anyX 200MA 100V Zener Diode (1N4148): This diode will be used as an additional protection diode for the RF module.
 * - anyX Metal Film Resistors (1/4w, any value): This resistors will be used as pull-up and pull-down resistors for the buttons and switches, also for resistor ladder to combine the buttons.
 * 
 * @note
 * All the components listed above are based on what i have curently, while i believe that these components are needed, they may not be required for the project.
 * Some components may be added or removed based on the project requirements.
 * As for now, the listed will be my choice of components for the project.
 * 
 * @details
 * Abstract of how the joystick will be implemented (How it should look like):
 * - The KY-023 Joystick are 2-axis with a push button for each joystick. They requires 5v power input, meaning the output will be 5v too, which is a problem for the ESP32 whose operates at 3.3v. To solve this problem, we will use a voltage divider to convert the 5v output to 3.3v input for the ESP32.
 *   > The voltage divider will be made of two resistors, one connected to the 5v output and the other connected to the ground. The output of the voltage divider will be connected to the ESP32's analog input pin.
 *   > To determine the values of the resistors, we will use the formula: Vout = Vin * (R2 / (R1 + R2)), where Vout is the output voltage, Vin is the input voltage, R1 is the resistor connected to the input voltage, and R2 is the resistor connected to the ground.
 *   > Meaning, we will use 2 resistors of 1k and 2k to convert the 5v output to 3.3v input for the ESP32.
 *   > The highest value resistor (2k) will be connected to the 5v output, and the lowest value resistor (1k) will be connected to the ground. The output of the voltage divider will be connected to the ESP32's analog input pin.
 * - Each push buttons can operates at whatever voltage, but we will use 3.3v for the ESP32. The push buttons will be connected to the ESP32's digital input pins.
 *   > Issue here is that, there are too many buttons (8 in total), and the ESP32 will run out of GPIO pins to use before we could implement the rest of the components.
 *   > To solve this, we will use a resistor ladder to combine the buttons into a single analog input pin.
 *   > The setup of the ladder is to create 2 separate groups of buttons, each group will have 4 buttons, and each group will be connected to a single analog input pin.
 *   > Reason why we are using resistor ladder for the buttons is that, not only it will saves GPIO pins, it's also a consideration of how our Thuumbs limited to only press a button at a time for each side, so using independent GPIO pins for each button is not necessary.
 *   > The resistor ladder will be made of 4 resistors, each connected to a button, and the output of the ladder will be connected to the ESP32's analog input pin.
 *   > For this ladder, we will use the most simple setup of buttons-resistors-buttons-resistors, where each button is connected to a resistor, and the output of the ladder is connected to the ESP32's analog input pin.
 *   > Some simple graphs to illustrate the setup:
 *   > 
 *   >    [3.3V]
 *   >       |
 *   >       +---[Button 1]---[1k]---+
 *   >       |                       |
 *   >       +---[Button 2]---[2.2k]-+
 *   >       |                       |
 *   >       +---[Button 3]---[3.3k]-+----> [Analog Input Pin]
 *   >       |                       |
 *   >       +---[Button 4]---[4.7k]-+
 *   >                               |
 *   >                             [10k]
 *   >                               |
 *   >                              GND
 *   > Here's why it works:
 *   > The output of the ladder will be a voltage that is proportional to the number of buttons pressed, and the value of the resistors will determine the voltage output.
 *   > For example, if Button 1 is pressed, the output will be 3.3v, if Button 2 is pressed, the output will be 2.2v, if Button 3 is pressed, the output will be 1.65v, and so on.
 *   > This is the most standard way of implementing a resistor ladder for buttons, and it will work with the ESP32's analog input pins.
 *   > And it does saves us GPIO pins, as we only need 2 analog input pins for the 8 buttons.
 * - The limit switches that used as trigger buttons will be connected to the ESP32's digital input pins.
 *   > The limit switches doesn't require any voltage divider or resistor ladder, as they are just simple switches that will connect the input pin to ground when pressed.
 *   > Another reason for that is due to the fact that we will be using 2 fingers for the trigger, adn simultanous input is to be expected, so using independent GPIO pins for each switch is necessary.
 * - The 18650 Battery Shield will be used to power the ESP32 and the other components.
 *   > The shield will provide 3.3v and 5v power simultaneously to the joystick.
 *   > The ESP32 will be powered by the 3.3v output, and the KY-023 Joystick will be powered by the 5v output.
 * - The 18650 Battery will be used to power the joystick.
 *   > The battery will be connected to the 18650 Battery Shield, and it will provide power to the joystick.
 * - The 0.66-inch White OLED Display Shield will be used to show information on the joystick.
 *   > The display will be connected to the ESP32's I2C pins, and it will be used to show information such as the joystick position, battery level, and other information.
 * - The NRF24L01+PA/LNA RF Module will be used to communicate with the PC.
 *   > The module will be connected to the ESP32's SPI pins, and it will be used to send and receive data from the PC.
 * - The Passive Buzzer will be used to provide audio feedback for the joystick.
 *   > The buzzer will be connected to the ESP32's PWM pin, and it will be used to provide audio feedback for the joystick.
 * - The 2S Battery Indicator will be used to indicate the battery level of the joystick.
 *   > This one requires us to directly connect the wires onto the battery, as the shield does not have any battery indicator.
 *   > The indicator will be connected to the battery, and it will be used to indicate the battery level of the joystick.
 * 
 * @note
 * This sketch is a plan and does not contain complete code. It is meant to be a starting point for further development and implementation.
 */