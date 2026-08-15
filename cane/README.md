# PathFinder Cane

## Project structure
The cane source code can be split up into logical parts:
- Hardware interfaces: [tof.cpp](./src/tof.cpp), [motor.cpp](./src/motor.cpp),  [speaker.cpp](./src/speaker.cpp), [camera.cpp](./src/camera.cpp), and [imu.cpp](./src/imu.cpp) (currently unused)
- Wireless technologies: [websocket.cpp](./src/websocket.cpp)
- Core runtime logic: [runtime.cpp](./src/runtime.cpp), [src.ino](./src/src.ino)
- Compile-time configuration: [config.hpp](./include/config.hpp)

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

Please note that many boards will not be supported due to the dependencies and requirements.

## Documentation
This is an overview and explanation of the code.

Each subsytem has its own `*_setup()` and `*_loop()` functions, and the rest of the functions used are internal to the system. Each subsytem's public objects are globally declared in [main.hpp](./include/main.hpp) for simplicity (though not great design), and constructed globally in [main.cpp](./src/main.cpp).

### Hardware interfaces
#### Motors
PathFinder uses two pins to control the left and right motors of the cane. We use the built-in `analogWrite` of the Arduino library, but via a [wrapper](./include/motor.hpp).

The pin numbers can be found in the [configuration file](./include/config.hpp).

#### ToF Sensors
PathFinder uses two Time-of-Flight LiDAR sensors by DFRobot, and can be found [here](https://wiki.dfrobot.com/sen0628/#tech_specs). We use the [DFRobot MatrixLidar library](./libs/DFRobot_MatrixLidar-master.zip) to communicate to both over I2C.

The bulk of this logic can be found [here](./src/tof.cpp), and [here](./include/tof.hpp).

#### Camera
PathFinder uses an AI-powered camera module called the Grove Vision AI V2 by Seeed Studio, which can be found [here](https://wiki.seeedstudio.com/grove_vision_ai_v2/). This module is then connected to an OV5647 via a CSI cable. We use the [Seeed Arduino SSCMA library](./libs/Seeed_Arduino_SSCMA-main.zip) to communicate to the camera module over I2C.

The bulk of this logic can be found [here](./src/camera.cpp), and [here](./include/camera.hpp).

#### Speaker
PathFinder uses a DFPlayer Mini MP3 by DFRobot, which can be found [here](https://wiki.dfrobot.com/dfr0299/#tech_specs), alongside a single speaker connected to it over its SPK +/- pins. Additionally, the DFPlayer Mini has a FAT32 formatted SD card, the contents of which can be found [here](./data/sdcard/). We use the [DFRobotDFPlayerMini library](./libs/DFRobotDFPlayerMini-master.zip) to connect to the DFPlayer.

The DFPlayer uses a numbered system that is reflected in the [configuration file](./include/config.hpp) as well as the [SD card setup](./data/sdcard/).

For other subsystems to make use of the audio, they use the `audio_manager` object which has abstractions that allow you to queue and also completely play files via: `AudioManager::queue(uint8_t file)`, `AudioManager::step()`, and `AudioManager::wait()`

The bulk of this logic can be found [here](./src/speaker.cpp), and [here](./include/speaker.hpp).

#### IMU
While PathFinder does not yet use this code, we wrote it in considering the possibility of it being used. The code is written for a DFRobot BMI160 IMU, which can be found [here](https://wiki.dfrobot.com/sen0250/#tech_specs). We use the [DFRobot BMI160 library](./libs/DFRobot_BMI160-master.zip) to connect to it.

The bulk of this logic can be found [here](./src/imu.cpp), and [here](./include/imu.hpp).

### Wireless technologies
If you looked at a less recent commit, we previously had rudimentary audio support via Bluetooth A2DP, but that fell through. So our only wireless technology is WiFi currently.

#### Wifi / WebSocket clients
Before we can get up a server, the PathFinder connects to WiFi using the built-in Arduino WiFi library, you may change the SSID and password via [the configuration file](./include/config.hpp). PathFinder opens an asynchronous WebSocket server on port 8765, and exposes two different handlers:

##### The main handler/sensor data handler
One for the sensor data itself in addition to some diagnostics, which is accessed at the extension "/". This can also receive commands which will modify certain settings variables that are used for calculations in [the runtime](./src/runtime.cpp). By default, this data is sent at an interval of 500 milliseconds.

##### The preview handler
This handler sends camera output as a Base64 string that can then be rendered on the client side (dashboard). In order to receive a frame, the client sends a JSON command of the form `{"command" : "request_frame"}`. The dashboard sends this command at a constant rate to enforce a sort of frames-per-second, though this shouuld be kept to a minimum as iti s unreliable and can bottleneck the ESP32.

We use [ESPAsyncWebServer](./libs/ESPAsyncWebServer-3.11.2.zip) to start up the asynchronous server, and the WebSocket handlers using [AsyncTCP](./libs/AsyncTCP-main.zip).

The bulk of this logic can be found [here](./src/websocket.cpp), and [here](./include/websocket.hpp).

##### Handling race conditions
Because of the asynchronous nature of the server, we use atomic operations to handle the settings modified by the server via `std::atomic`, and a mutex via `SemaphoreHandle_t`, aptly named `invoke_mutex` in order to enforce synchronization with `camera_ai` when gathering the Base64 image string. If you do not use this model, you could be susceptible to race conditions, where one thread tries to read from a state **as it is being updated** by another thread, which can cause a myriad of problems / undefined behavior.

**PLEASE NOTE** This approach of using a semaphore to avoid race conditions could be very laggy. So laggy in fact, we decided to just run the risk of crashing every so often and commented out this safeguard on Demo Day (not recommended!). This is probably just my lack of critical thinking, but it seemed to me like this was the best way to avoid race conditions here, although other methods you think up could definitely fare better.

### Core runtime logic
The core logic of the PathFinder program

#### Runtime
PathFinder uses trigonometry to estimate the position of objects in 3D space based off of the two ToF sensors, `tof_left` and `tof_right`, as well as the bounding boxes provided by the camera, using `camera_ai.boxes()`.

The estimated 3D points get pushed into a buffer called `point_cloud`, which is then processed to apply motor intensity values to the left and right motors.

The bounding boxes provided by the camera get converted to estimated 2.5D boxes (no thickness), based on the closest match we can find to a depth value in the point cloud that was previously generated. The closest bounding box is then used to perform audio cues, indicating class/target (person), distance (close, far), and direction (left, right, ahead).

The bulk of this logic can be found [here](./src/runtime.cpp), and [here](./include/runtime.hpp).

#### Main
In [main.cpp](./src/main.cpp), we do bare-bones level configurations. We initialize the `Serial` object, and set up a way to read for the reset pin, so we can perform software-level resetting.

#### Entry point
As per most Arduino projects, the entry point starts at `setup()` inside of [src.ino](./src/src.ino).

We setup all of the subsystems, then we go into `loop()`, which runs each of the subsytems' iterations, similar to `setup()`.

### Compile-time configuration
The [configuration file](./include/config.hpp) file is where compile-time configuration is done. You can change defaults, pins, angles of sensors and the camera, etc. Note that this is a sub-optimal way to perform changes, and you can use the [dashboard](../dashboard/) if you need a faster way to tune values.

