# Petduino
Provides a collection of helper methods for creating animations, melodies and interactions with a Petduino.

## Installation
1. Download and install the latest version of the [Arduino IDE](http://www.arduino.cc/en/main/software).
2. Download and install the latest [CH340G chip drivers](http://www.wch.cn/downloads.php?name=pro&proid=65).
3. Download the latest [LedControl arduino library](https://github.com/wayoda/LedControl/releases).
4. Install the LedControl library using [the standard Arduino library install procedure](http://www.arduino.cc/en/Guide/Libraries#.UwxndHX5PtY).
5. Download this library using this link (TBC).
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
TODO
