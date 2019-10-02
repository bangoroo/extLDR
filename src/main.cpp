#include "main.h"

//define clients
WiFiClient espClient;
PubSubClient client(espClient);

#define ldrPower D8
/********************************** START SETUP WIFI*****************************************/
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
   Serial.print("MAC: ");
   Serial.println(WiFi.macAddress());
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    ESP.wdtFeed();
    delay(500);
    yield();
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/********************************** START RECONNECT*****************************************/
void reconnect() {
  // Loop until we're reconnected
  byte trys = 0;
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(SENSORNAME, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      trys = 0;

      ESP.wdtFeed();
      delay(500);    

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying
      trys +=1;
      Serial.print("Trys: ");
      Serial.println(trys);
      if(trys >=10){
        Serial.println("Reach maximum of trys. Restarting ESP...");
        ESP.restart();
      }
      
      yield();
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(D1,INPUT);
  pinMode(ldrPower, OUTPUT);
  //Setup Wifi & mqtt
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  //client.setCallback(callback);
  Serial.println("Client ready");
  DEBUG_MSG("Client ready\n");

  Serial.println("Ready");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

 //reconnect
  if (!client.connected()) {
    reconnect();
  }
  //check Wifi state
  if (WiFi.status() != WL_CONNECTED) {
    delay(1);
    yield();
    Serial.print("WIFI Disconnected. Attempting reconnection.");
    setup_wifi();
    return;
  }
  ESP.wdtFeed();
  client.loop();
  digitalWrite(ldrPower, HIGH);
  delay(1000);
  lightState = digitalRead(LDR_PIN);
  
  sendToMqtt("extLDR",(bool)lightState,light_set_topic);

  
  delay(5000);
  digitalWrite(ldrPower, LOW);
  Serial.println("Going to sleep Zzz");
  Serial.end();
  ESP.deepSleep(300*1000000);
  ESP.deepSleepMax();
}

void loop() {
  // put your main code here, to run repeatedly:
  

}

/**********************************Send to MQTT ***********************************************/
void sendToMqtt(String path, bool val, const char* destinationTopic){
  DynamicJsonDocument jsonDoc(1024);
  //JsonObject root = jsonDoc.as<JsonObject>(); 
  //button state
  Serial.print("Sending Mesage: ");
  Serial.print(path);
  Serial.print(" -> ");
  Serial.println(val);
  // root[path] = val;
  jsonDoc[path] = val;

  //char buffer[measureJson(jsonDoc) + 1];
  char buffer[512];
  //root.printTo(buffer, sizeof(buffer));
  serializeJson(jsonDoc, buffer /* ,sizeof(buffer)*/);
  yield();
  

  client.publish(destinationTopic, buffer, true);
  Serial.println("Message sent");
  
 
}
