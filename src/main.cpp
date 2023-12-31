#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#endif // ESP8266
#ifdef ESP32 
#include <WiFi.h>
#endif // ESP32
#include <PubSubClient.h>
#include <EEPROM.h>


// Fan control pin. 16 is D0 on esp8266. Also blinks the onboard LED.
const int FAN_PWM_PIN = 16;

// Relay control pin. 5 is D1 on esp8266.
const int RELAY_PIN = 5;

WiFiClient espClient;
PubSubClient client(espClient);

uint8_t current_speed = 30; // Default speed
uint8_t is_on = 1; // Is the fan on? By default its off


void callback(char* topic, byte* payload, unsigned int length) {

    Serial.print("Message received");
    Serial.print("  topic: ");
    Serial.print(topic);
    Serial.print("  payload: ");
    Serial.println("");

    for (int i = 0; i < (int)length; i++) {
        Serial.print((char)payload[i]);
    }

    Serial.println("   length: " + String(length));
    Serial.println("");


    if (length == 0 || length > 3) {
        Serial.println("Invalid payload length");
        return;
    }

    // Check if message is 'on' or 'off'
    if (
        length == 2 && 
        (
            (payload[0] == 'o' && payload[1] == 'n') ||
            (payload[0] == 'O' && payload[1] == 'N')
        )
    ) {
        Serial.println("Turning on");

        EEPROM.write(2, 0);
        EEPROM.commit();

        // Turn on the fan with a speed burst to get it started
        analogWrite(FAN_PWM_PIN, 200);
        digitalWrite(RELAY_PIN, 0);     // Turn on the relay
        delay(750);
        analogWrite(FAN_PWM_PIN, current_speed);   // "Normal" IDLE speed
        return;
    }

    if (
        length == 3 && 
        (
            (payload[0] == 'o' && payload[1] == 'f' && payload[2] == 'f') ||
            (payload[0] == 'O' && payload[1] == 'F' && payload[2] == 'F')
        )
    ) {
        Serial.println("Turning off");

        EEPROM.write(2, 1);
        EEPROM.commit();

        digitalWrite(RELAY_PIN, 1);
        return;
    }


    // Check if the payload is a number
    for (int i = 0; i < (int)length; i++) {
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

    // Set the fan 
    current_speed = speed;
    EEPROM.write(1, current_speed);
    EEPROM.commit();
    analogWrite(FAN_PWM_PIN, current_speed);

}

void reconnect() {

    while (!client.connected()) {

        Serial.println("Connecting to MQTT server");

        String clientId = "ceilingfan-4f3e8b59";

        if (client.connect(clientId.c_str())) {

            Serial.println("Connected to MQTT server!");

            // Tell that we are online
            client.publish("status", "online");
            client.publish("version", "0.3");

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

    EEPROM.begin(512);

    if (EEPROM.read(0) != 42) {
        // Init EEPROM
        EEPROM.write(0, 42);
        EEPROM.write(1, current_speed);
        EEPROM.write(2, 0);             // Is my fan on? 0 = off, 1 = on
        EEPROM.commit();
    } else {
        current_speed = EEPROM.read(1);
        // Ensure that unknown value means that the fan is off
        is_on = EEPROM.read(2) == 0 ? 0 : 1;
    }

    pinMode(FAN_PWM_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    analogWrite(FAN_PWM_PIN, current_speed);
    digitalWrite(RELAY_PIN, is_on);

    // Since this project is meant to be used with ESP8266, we can use the
    // ESP32 to check if this is compiled for simulation therefore requiring
    // to use Wokwi guest network
    #ifdef ESP8266
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    #else
    WiFi.begin("Wokwi-GUEST", "", 6);
    #endif // ESP8266
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to network..." + String(WIFI_SSID) + ":::" + String(WIFI_PASSWORD));
    }

    Serial.println("Connected to the WiFi network");

    client.setServer(MQTT_SERVER_ADDRESS, 1883);

    client.setCallback(callback);


    #ifdef ESP8266
    ArduinoOTA.setHostname("ceilingfan-4f3e8b59");
    // ArduinoOTA.setPassword((const char *)"qwerty");
    ArduinoOTA.begin();
    #endif // ESP8266

}

void loop() {

    delay(10);

    #ifdef ESP8266
    ArduinoOTA.handle();
    #endif // ESP8266


    if (!client.connected()) {
        reconnect();
    }

    client.loop();

}
