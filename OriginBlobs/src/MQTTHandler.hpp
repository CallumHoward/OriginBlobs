// MQTTHandler.hpp
// Callum Howard, 2020

#ifndef MQTTHANDLER_HPP
#define MQTTHANDLER_HPP

#include <functional>  // bind
#include <string>

#include <WiFiClient.h>
#include <PubSubClient.h>
#include "Credentials.hpp"
#include "Utils.hpp"  // log

namespace ch {

class MQTTHandler {
public:
    MQTTHandler(const std::function<void()>& pulseCallback) : mPulseCallback{pulseCallback} {
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

    void callback(char* inTopic, byte* payload, unsigned int length) {
        const auto topic = std::string(inTopic);
        const auto message = std::string(reinterpret_cast<char*>(payload), length);
        if (topic == "pulse") {
            mPulseCallback();
        } else if (topic == "rollCall") {
            mPubSubClient.publish(topic.c_str(), getId().c_str());
        } else if (topic == "assign" && message.rfind(getId(), 0)) {
            mSoulMate = std::string{message.cbegin() + getId().size() + 1, message.cend()};
        } else if (topic == "update") {
            mUpdateCallback();
        }
    }

    std::string getId() {
        // The chip ID is essentially its MAC address(length: 6 bytes).
        uint64_t chipId = ESP.getEfuseMac();
        const size_t bufferSize = 6;
        char messageBuffer[bufferSize];
        snprintf(messageBuffer, bufferSize, "%04X%08X",
                static_cast<uint16_t>(chipId >> 32),    // High 2 bytes
                static_cast<uint32_t>(chipId));         // Low 4 bytes
        return std::string{messageBuffer, bufferSize};
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
                mPubSubClient.publish("newlyConnected", "hello world");
                // ... and resubscribe
                mPubSubClient.subscribe("rollCall");
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
        if (!mPubSubClient.connected()) { reconnect(); }
        mPubSubClient.loop();

        unsigned long currentMillis = millis();
        if (currentMillis - mPreviousMillis > mInterval) {
            mPreviousMillis = currentMillis;
            ++mCounter;
            mPubSubClient.publish("ping", getId().c_str());
        }
    }

    void notifyPulseSoulMate() {
        snprintf(mMessageBuffer, sBufferSize, "pulse");
        mPubSubClient.publish(mSoulMate.c_str(), mMessageBuffer);
    }

private:
    WiFiClient mWifiClient;
    PubSubClient mPubSubClient;

    const std::function<void()> mPulseCallback;
    std::function<void()> mUpdateCallback;

    std::string mSoulMate;

    static constexpr size_t sBufferSize = 50;
    char mMessageBuffer[sBufferSize];
    unsigned long mPreviousMillis = 0;
    const unsigned long mInterval = 5000;
    int mCounter = 0;

    ch::Logger log;
};

} // namespace ch

#endif