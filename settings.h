// *************************
//          Wheels
// *************************
const int FrontRight = 16;
const int BackRight = 17;
const int FrontLeft = 5;
const int BackLeft = 18;
int vTurn = LOW;

// *************************
//    Speed ofset wheels
// *************************
int speedL = 214;
int speedR = 233;
// float speedL = 180;
// float speedR = speedL-19;
const int v = 190; 
//const int v = 150;

//*************

const int IRLeft = 34;
const int IRRight = 39;
unsigned long previousMillis = 0;
const long interval = 150;
// Wifi Credentials
const char* ssid = "Wifi is saai"; // Wifi SSID
const char* password = "2much4u2day"; //Wi-FI Password

// const char* webSocketHost = "10.110.111.103";
// const char* webSocketHost = "battlebot1.serverict.nl";
const char* webSocketHost = "192.168.179.155";
const int webSocketPort = 33003;
