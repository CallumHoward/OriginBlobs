// MQTTHandler.hpp
// Callum Howard, 2020

#ifndef MQTTHANDLER_HPP
#define MQTTHANDLER_HPP

#include <functional>  // bind
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "Credentials.hpp"

namespace ch {

class MQTTHandler {
public:
    MQTTHandler() {
        mWifiClient = WiFiClient{};
        mPubSubClient = PubSubClient{mWifiClient};

        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }

        randomSeed(micros());

        const auto callbackFunction = std::bind(&ch::MQTTHandler::callback,
                this, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3);

        mPubSubClient.setServer(mqttServer, mqttPort);
        mPubSubClient.setCallback(callbackFunction);
    }

    void callback(char* topic, byte* payload, unsigned int length) {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");
        for (int i = 0; i < length; i++) {
            Serial.print((char)payload[i]);
        }
        Serial.println();

        //// Switch on the LED if an 1 was received as first character
        //if ((char)payload[0] == '1') {
        //    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
        //    // but actually the LED is on; this is because
        //    // it is active low on the ESP-01)
        //} else {
        //    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
        //}

    }

    void reconnect() {
        // Loop until we're reconnected
        while (!mPubSubClient.connected()) {
            Serial.print("Attempting MQTT connection...");
            // Create a random mPubSubClient ID
            String clientId = "ESP32Client-";
            clientId += String(random(0xffff), HEX);
            // Attempt to connect
            if (mPubSubClient.connect(clientId.c_str())) {
                Serial.println("connected");
                // Once connected, publish an announcement...
                mPubSubClient.publish("outTopic", "hello world");
                // ... and resubscribe
                mPubSubClient.subscribe("inTopic");
            } else {
                Serial.print("failed, rc=");
                Serial.print(mPubSubClient.state());
                Serial.println(" try again in 5 seconds");
                // Wait 5 seconds before retrying
                delay(5000);
            }
        }
    }

    void update() {
        if (!mPubSubClient.connected()) {
            reconnect();
        }
        mPubSubClient.loop();

        long now = millis();
        if (now - mLastMessage > 2000) {
            mLastMessage = now;
            ++mCounter;
            snprintf(mMessageBuffer, 50, "hello world #%d", mCounter);
            Serial.print("Publish message: ");
            Serial.println(mMessageBuffer);
            mPubSubClient.publish("outTopic", mMessageBuffer);
        }
    }

private:
    WiFiClient mWifiClient;
    PubSubClient mPubSubClient;
    char mMessageBuffer[50];
    long mLastMessage = 0;
    int mCounter = 0;
};

} // namespace ch

#endif
