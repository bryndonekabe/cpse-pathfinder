# PathFinder Cane

## Project structure
The cane source code can be split up into logical parts:
- Hardware interfaces: `tof.cpp`, `motor.cpp`,  `speaker.cpp`, `camera.cpp`, and `imu.cpp`(currently unused)
- Wireless technologies:`websocket.cpp`
- Core runtime logic:`runtime.cpp`, `src.ino`
- Compile-time configuration:`config.hpp`

## Documentation
This is an overview and explanation of the code.

Each subsytem has its own `*_setup()` and `*_loop()` functions, and the rest of the functions used are internal to the system. Each subsytem's public objects are globally declared in `main.hpp` for simplicity (though not great design), and constructed globally in `main.cpp`.

### Hardware interfaces
#### Motors
PathFinder uses two pins to control the left and right motors of the cane. We use the built-in `analogWrite` of the Arduino library, but via a [wrapper](./include/tof.hpp).

The pin numbers can be found in the [configuration file](./include/config.hpp).

#### ToF Sensors
PathFinder uses two Time-of-Flight sensors by DFRobot, and can be found [here](https://wiki.dfrobot.com/sen0628/#tech_specs). We use the [DFRobot MatrixLidar library](./libs/DFRobot_MatrixLidar-master.zip) to communicate to both over I2C.

The bulk of this logic can be found [here](./src/tof.cpp), and [here](./include/tof.hpp).

#### Camera
PathFinder uses an AI-powered camera module called the Grove Vision AI V2 by Seeed Studio, which can be found [here](https://wiki.seeedstudio.com/grove_vision_ai_v2/). This module is then connected to an OV5647 via a CSI cable. We use the [Seeed Arduino SSCMA library](./libs/Seeed_Arduino_SSCMA-main.zip) to communicate to the camera module over I2C.

The bulk of this logic can be found [here](./src/camera.cpp), and [here](./include/camera.hpp).

#### Speaker
PathFinder uses a DFPlayer Mini MP3 by DFRobot, which can be found [here](https://wiki.dfrobot.com/dfr0299/#tech_specs), alongside a single speaker connected to it over SPK +/-. Additionally, the DFPlayer Mini has a FAT32 formatted SD card, the contents of which can be found [here](./data/sdcard/). We use the [DFRobotDFPlayerMini library](./libs/DFRobotDFPlayerMini-master.zip) to connect to the DFPlayer.

The DFPlayer uses a numbered system that is reflected in the [configuration file](./include/config.hpp) as well as the [SD card setup](./data/sdcard/).

For other subsystems to make use of the audio, they use the `audio_manager` object which has abstractions that allow you to queue and also completely play files via: `AudioManager::queue(uint8_t file)` and `AudioManager::wait()`

The bulk of this logic can be found [here](./src/speaker.cpp), and [here](./include/speaker.hpp).

#### IMU
While PathFinder does not yet use this code, we wrote it in considering the possibility of it being used. The code is written for a DFRobot BMI160 IMU, which can be found [here](https://wiki.dfrobot.com/sen0250/#tech_specs). We use the [DFRobot BMI160 library](./libs/DFRobot_BMI160-master.zip) to connect to it.

The bulk of this logic can be found [here](./src/imu.cpp), and [here](./include/imu.hpp).

### Wireless technologies
#### Wifi / WebSocket clients
First, the PathFinder connects to WiFi using the built-in Arduino WiFi library. PathFinder uses two asynchronous WebSocket clients, one for the sensor data itself + some diagnostics, and another to send the raw camera output as a Base64 string that can then be rendered on the client side (dashboard). We use [ESPAsyncWebServer](./libs/ESPAsyncWebServer-3.11.2.zip) to start up the asynchronous server, and the asynchronous WebSocket clients using [AsyncTCP](./libs/AsyncTCP-main.zip).

The bulk of this logic can be found [here](./src/websocket.cpp), and [here](./include/websocket.hpp).

### Core runtime logic
#### Runtime
PathFinder uses trigonometry to estimate the position of objects in 3D space based off of the two ToF sensors, `tof_left` and `tof_right`, as well as the bounding boxes provided by the camera, using `camera_ai.boxes()`.

The estimated 3D points get pushed into a buffer called `point_cloud`, which is then processed to apply motor intensity values to the left and right motors.

The bounding boxes provided by the camera get converted to estimated 2.5D boxes (no thickness), based on the closest match we can find to a depth value in the point cloud that was previously generated. The closest bounding box is then used to perform audio cues, indicating class/target (person), distance (close, far), and direction (left, right, ahead).

The bulk of this logic can be found [here](./src/runtime.cpp), and [here](./include/runtime.hpp).

### Compile-time configuration
The [configuration file](./include/config.hpp) file is where compile-time configuration is done. You can change defaults, pins, etc. Note that this is a sub-optimal way to perform changes, and you can use the [dashboard](../dashboard/) if you need a faster way you tune values.

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
