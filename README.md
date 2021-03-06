# Petduino
Provides a collection of helper methods for creating animations, melodies and interactions with a Petduino.

## Installation
1. Download and install the latest version of the [Arduino IDE](http://www.arduino.cc/en/main/software).
2. Download and install the latest [CH340G chip drivers](http://www.wch.cn/downloads.php?name=pro&proid=65).
3. Download the latest [LedControl arduino library](https://github.com/wayoda/LedControl/releases).
4. Install the LedControl library using [the standard Arduino library install procedure](http://www.arduino.cc/en/Guide/Libraries#.UwxndHX5PtY).
5. Download the latest Petduino library from the [Releases page](https://github.com/circuitbeard/petduino/releases).
6. Install this library using [the standard Arduino library install procedure](http://www.arduino.cc/en/Guide/Libraries#.UwxndHX5PtY).

## Getting Started
Every Petduino project starts with the same basic code structure.

    #include <LedControl.h>
    #include <Petduino.h>

    Petduino pet = Petduino();

    void setup() {

      // Setup Petduino
      pet.begin();

    }

    void loop() {

      // Update pet
      pet.update();

      // Do your thing...

    }

From here Petduino exposes a number of methods to help you handle interactions from your Petduinos various inputs, methods to help you draw graphics to the Petduinos display and a simple state manager to help create non blocking animations.

To explore all the things you can do with your Petduino, checkout the various examples in the [examples folder](https://github.com/circuitbeard/petduino/tree/master/examples).

## Examples
| Example | Description |
| ------- | ----------- |
| **AlienDelay** | Basic example to show rendering graphics to the display and playing simple tones. |
| **AlienState** | The same as the AlienDelay example, however this time showing the use of Petduinos state manager to create not blocking animations. |
| **ButtonExample** | Example project showing how to interact with the Petduinos buttons.  |
| **LdrExample** | Example of reading the Petduinos light level (LDR) sensor. |
| **TemperatureExample** | Example of how to read the temperature from your Petduino. |
| **ScrollingText** | Example of how to turn your Petduino into a scrolling text display. |
| **TheEye** | Aadvanced example showing how to render dynamic grphics to the Petduinos display. |
| **Pets** | A collection of simple pet face to give your Petduino instant personality and a starting point to add your own stamp. Incorporating animations, sounds and button inteactions. |
| **Games** | A collection of games to show how you can do more with your Petduino than just animations. |
| **Instruments** | A collection of musical instrument examples for the Petduino. |

## Contributing
TODO
