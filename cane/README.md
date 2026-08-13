# PathFinder Cane

## Project structure
The cane source code can be split up into parts:
- Hardware (`tof.cpp`, `motor.cpp`, `imu.cpp`, `speaker.cpp`)
- Wireless technologies (`websocket.cpp`)
- Core runtime logic (`runtime.cpp`)
- Compile-time configuration (`config.hpp`)

## Prerequisites
- Arduino IDE and/or corresponding `arduino-cli` commands

If using `arduino-cli`, please use those corresponding commands, as this mainly covers Arduino IDE.

## Installing dependencies
To install the libraries the code uses, follow these instructions:
1. Open Arduino IDE
2. Navigate to the top toolbar
3. Select Sketch -> Include library -> Add .ZIP library
4. Navigate to the `libs` folder inside the `cane` directory, and select your desired .ZIP file
5. Repeat for each library you need to install

## Compiling and running
To compile the cane code, simply open the project, then click the checkmark icon in the top-left of the Arduino IDE, this will compile the code.

To run the code, simply click on the arrow that is to the right of the checkmark icon, this will flash the compiled code to your connected microcontroller. 

Please note that many boards will not be supported.
