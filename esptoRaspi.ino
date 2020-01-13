#include <PubSubClient.h>
#include <WiFi.h>

const int sensor_pin = 39;
const char* ssid = "firstfloor";                   // wifi ssid
const char* password =  "cdac@pune";         // wifi password
const char* mqttServer = "192.168.1.25";    // IP adress Raspberry Pi
const int mqttPort = 1883;
const char* mqttUser = "";      // if you don't have MQTT Username, no need input
const char* mqttPassword = "";  // if you don't have MQTT Password, no need input

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(115200);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

//  client.publish("esp32", "Hello Raspberry Pi");
//  client.subscribe("esp32");

}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}

void loop() {

  char moisture[8];

  double moisture_percentage = ( (analogRead(sensor_pin)/4095) * 100);
 
 
  dtostrf(moisture_percentage,5,2,moisture);

//  Serial.print("Soil Moisture(in Percentage) = ");
//  Serial.print(moisture_percentage);
//  Serial.println("%");

  delay(1000);
    client.publish("esp32",moisture);
    client.subscribe("esp32");
    delay(300);
  client.loop();
}
