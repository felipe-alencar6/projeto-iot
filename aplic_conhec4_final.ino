#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Cafe_116";
const char* password = "Cafe12121212";
const char* mqtt_server = "test.mosquitto.org";
const char* topic = "sound_data";

const int soundSensorPin = 32;  // Replace with the pin your sound sensor is connected to

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(soundSensorPin, INPUT);

  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  
  int soundLevel = analogRead(soundSensorPin);

  Serial.print("Sound Level: ");
  Serial.println(soundLevel);

  if (client.publish(topic, String(soundLevel).c_str(),1)) {
    Serial.println("Publish success");
  } else {
    Serial.println("Publish failed");
  }

  delay(1000);
}
