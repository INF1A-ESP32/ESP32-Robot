# Installing libraries into the Arduino IDE

Go to `Sketch->Include Library->Manage Libraries...` and install the following packages:

|           Library | Version | Author                | Used for                                                         |
| ----------------: | :-----: | --------------------- | ---------------------------------------------------------------- |
| ESP32 AnalogWrite |  0.1.0  | Abdelouahed ERROUAGUY | Use analog write on the ESP32 because there is no native support |
|  Adafruit_VL53L0X |  1.1.3  | adafruit              | For the Proximity sensor to read the values                      |
|  Adafruit_SSD1306 |  2.5.1  | Adafruit              | Make stuff display on the                                        |
|        Websockets |  2.3.5  | Markus Sattler        | Websocket communication between the robot and the server         |
|       ArduinoJson | 6.19.2  | Benoit Blanchon       | Parsing the incoming JSON data                                   |
|  MPU9250_asukiaaa | 1.5.11  | Asuki Kono            | Reads the acceleration of the MPU chip                           |

# Added the ESP32 to the arduino boards
In your Arduino IDE, go to `File->Preferences` and add the following to `Additional Boards Manager URLs:` `https://dl.espressif.com/dl/package_esp32_index.json`

After that install the board trough `Tools->Board-> Boards manager...` and search for "ESP32" by Espressif Systems

# Upload the code
`Tools->Board` and select the `DOIT ESP32 DEVKIT V1` board now you can upload the code to the esp

# Settings
To make the robot work make sure that you change the settings in the file `settings.h` like the websocket host and the wifi ssid and password