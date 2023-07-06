#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// Fan control pin. 16 is D0 on esp8266. Also blinks the onboard LED.
const int FAN_PWM_PIN = 16;

// WIFI credentials
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

// MQTT server address and port
const char* MQTT_SERVER_ADDRESS = "";
const int MQTT_SERVER_PORT = 1883;


WiFiClient espClient;
PubSubClient client(espClient);


void callback(char* topic, byte* payload, unsigned int length) {

    Serial.print("Message received");
    Serial.print("  topic: ");
    Serial.print(topic);
    Serial.print("  payload: ");
    Serial.println("");

    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();


    if (length == 0 || length > 3) {
        Serial.println("Invalid payload length");
        return;
    }

    // Check if the payload is a number
    for (int i = 0; i < length; i++) {
        if (payload[i] < '0' || payload[i] > '9') {
            Serial.println("Invalid payload");
            return;
        }
    }

    // Create a null-terminated string from the payload
    char speedStr[4]; // max 3 digits plus null terminator
    memcpy(speedStr, payload, length);
    speedStr[length] = '\0';

    // Convert the payload to an integer
    int speed = atoi(speedStr);

    // Check if the speed is in the valid range
    if (speed < 0 || speed > 255) {
        Serial.println("Invalid speed");
        return;
    }

    // Set the fan speed
    analogWrite(FAN_PWM_PIN, speed);

}

void reconnect() {

    while (!client.connected()) {

        Serial.println("Connecting to MQTT server");

        String clientId = "ceilingfan-4f3e8b59";

        if (client.connect(clientId.c_str())) {

            Serial.println("Connected to MQTT server!");

            // Tell that we are online
            client.publish("status", "online");

            // Subscribe to the control topic to receive commands
            // from the server to control the fan speed
            client.subscribe("control");

        } else {
            Serial.println("Could not connect to MQTT server. Retrying in 10 seconds..");
            delay(10000);
        }
    }
}


void setup() {

    Serial.begin(9600);

    pinMode(FAN_PWM_PIN, OUTPUT);
    analogWrite(FAN_PWM_PIN, 30);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting to WiFi..");
        delay(500);
    }

    Serial.println("Connected to the WiFi network");

    client.setServer(MQTT_SERVER_ADDRESS, MQTT_SERVER_PORT);

    client.setCallback(callback);

}

void loop() {

    if (!client.connected()) {
        reconnect();
    }

    client.loop();

}
