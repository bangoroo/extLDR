
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Arduino.h>

/****************************************FOR Sensors***************************************/
//PIN of PIR
#define PIR_PIN D0

//PIN LDR
#define LDR_PIN D1

//PIN Sound sensor
#define MIC_PIN D5

//PIN DHT11
#define TEMP_PIN D1

//PIN BUTTON
#define BUTTON_PIN D7

//PIN BUZZER
#define BUTTON_LED_PIN D8

//Type temperature sensor
#define DHTTYPE DHT11   // DHT 11


/************ WIFI and MQTT Information (CHANGE THESE FOR YOUR SETUP) ******************/
const char* ssid = "SSID"; //type your WIFI information inside the quotes
const char* password = "***********";
const char* mqtt_server =  "openhabianpi.local";
const char* mqtt_username = "openhabian";
const char* mqtt_password = "*******";
const int mqtt_port = 1883;

/**************************** FOR OTA **************************************************/
#define SENSORNAME "extLDR1" //change this to whatever you want to call your device
//#define OTApassword "OTA" //the password you will need to enter to upload remotely via the ArduinoIDE
//int OTAport = 8266;

/************* MQTT TOPICS (change these topics as you wish)  **************************/
const char* light_state_topic = "home/FF_Bedroom_LED_1";
const char* light_set_topic = "home/FF_Bedroom_LED_1/set";
const char* light_notify_topic = "home/FF_Bedroom_LED_1/notify";

const char* temp_state_topic = "home/FF_Bedroom_TEMP_1";
const char* temp_set_topic = "home/FF_Bedroom_TEMP_1/set";

//state ldr
int lightState = 0;//0 = no light, 1 = light


/****************************************FOR JSON***************************************/
const int BUFFER_SIZE = JSON_OBJECT_SIZE(10);
#define MQTT_MAX_PACKET_SIZE 512

//enable DEBUG
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif

void setup_wifi();
void reconnect();
void ldrCheck() ;
void sendToMqtt(String path, bool val, const char* destinationTopic);