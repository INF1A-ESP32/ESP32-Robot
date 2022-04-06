// *************************
//          Wheels
// *************************
int FrontRight = 17;
int BackRight = 16;
int FrontLeft = 18;
int BackLeft = 5;

// *************************
//    Speed ofset wheels
// *************************
int speedL = 233;
int speedR = 214;
// float speedL = 180;
// float speedR = speedL-19;
int vTurn = LOW;
const int v = 180;

//*************************
//        Display
//*************************
int IRLeft = 34;
int IRRight = 39;

unsigned long previousMillis = 0;
const long interval = 150;

// Wifi Credentials
const char* ssid = "Wifi is saai"; // Wifi SSID
const char* password = "2much4u2day"; //Wi-FI Password

// const char* webSocketHost = "10.110.111.103";
 const char* webSocketHost = "battlebot1.serverict.nl";
//const char* webSocketHost = "192.168.179.155";
const int webSocketPort = 33003;
