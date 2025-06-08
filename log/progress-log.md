# Project Progress Log

## LOG-1 | 05/06/25 | Abstract Planning
What's tthe fun in having a joystick but not constructing it yourself? DIY your way into creating it.
Here for i've added some approaches of how i will be starting this project, such as follows:
- Listing components on hands
- Creating abstract wiring
- Mkaing some considerations

I've already constructing some possible shapes of the joystick while at it and plan the buttons.

## LOG-2 | 06/06/25 | Refining the Plan
I noticed i've missed some issue regarding the KY-023 signal output handling, where i fed it 5v yet didn't apply the correct 3.3v voltage divider for it, so i fixed that and added some simple visualizations.
Along with that, i also experiment further the joystick, but it came out too bulky, and i was too lazy to pickup my power tools becuase it's late night already.
Some of the small components have arrived, so i guess i'll continue later.

## LOG-3 | 07/06/25 | Constructing the Base
While at it, i choose to map the pinout and review again the approach, and i do realizes that some of my input might interfere with the bootup of the ESP32.
I've made a modifications where for some specific pins, i connect it to the limit/micro switch to NO to get the pin HIGH at boot to avoid issue.
Since i choose NRF24L01 as the primary communication, i will disable WiFi and BLE features on the ESP32 in case of interference on some of the pins.
Now, i also attempted to construct the joystick by making some holes for the M3 PCB spacers and creates a custom layout, it came out nice.
But i've done something wrong, i don't know how to place the trigger pins to be comfortably usable, i'll think about that later.
Due to protoboard and buttons size constraints, i choose to make the dominant side to have the linear button layout while the other side have the D-pad/cross button layout.
Next i might started soldering them since the structure and layout already in place.

## LOG-4 | 08/06/25 | Resistive Divider and Revision
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