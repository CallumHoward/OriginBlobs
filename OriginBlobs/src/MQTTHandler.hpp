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
#include "OTAHandler2.hpp"

namespace ch {

String ipToString(IPAddress ip);

class MQTTHandler {
public:
    MQTTHandler(const std::function<void()>& pulseCallback) : mPulseCallback{pulseCallback} {
        mId = getId();
        mWifiClient = WiFiClient{};
        mPubSubClient = PubSubClient{mWifiClient};

        // Connect WIFI
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            log.info(".");
        }

        randomSeed(micros());

        const auto callbackFunction = std::bind(&ch::MQTTHandler::callback,
                this, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3);

        mPubSubClient.setServer(mqttServer, mqttPort);
        mPubSubClient.setCallback(callbackFunction);
    }

    void callback(char* inTopic, byte* payload, unsigned int length) {
        const auto topic = std::string{inTopic};
        const auto myUpdateTopic = std::string{"update/" + mId};
        const auto message = std::string(reinterpret_cast<char*>(payload), length);

        log.info("myUpdateTopic: ");
        log.info(myUpdateTopic.c_str());
        log.info("\ntopic: ");
        log.info(topic.c_str());
        log.info("\nmessage: ");
        log.info(message.c_str());
        log.info("\n");

        if (topic == "pulse") {
            mPulseCallback();
        } else if (topic == "rollCall" && message == "hello") {
            mPreviousMillis += mInterval;  // trigger update
            update();
        } else if (topic == "assign" && message.rfind(mId, 0)) {
            mSoulMate = std::string{message.cbegin() + mId.size() + 3, message.cend()};
        } else if (String{topic.c_str()} == String{myUpdateTopic.c_str()}) {
            log.info("update triggered\n");
            ch::execOTA(grpcServer, 2020, String{message.c_str()});
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
        const auto myUpdateTopic = "update/" + mId;

        // Loop until we're reconnected
        while (!mPubSubClient.connected()) {
            log.info("Attempting MQTT connection...");
            // Create a random mPubSubClient ID
            String clientId = "ESP32Client-";
            clientId += String(random(0xffff), HEX);
            // Attempt to connect
            if (mPubSubClient.connect(clientId.c_str())) {
                log.info("connected\n");
                // Once connected, publish an announcement...
                mPubSubClient.publish("newlyConnected", "hello world");
                // ... and resubscribe
                mPubSubClient.subscribe("rollCall");
                mPubSubClient.subscribe("pulse");
                mPubSubClient.subscribe("assign");
                mPubSubClient.subscribe(myUpdateTopic.c_str());
            } else {
                log.info("failed, rc=");
                log.info(mPubSubClient.state());
                log.info(" try again in 5 seconds\n");
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
            auto message = String{mId.c_str()} + String{","} +
                String{mType.c_str()} + String{","} +
                String{millis()} + String{","} +
                String{mCounter} + String{","} +
                String{mVersion.c_str()} + String{","} +
                ipToString(WiFi.localIP());
            mPubSubClient.publish("ping", message.c_str());
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

    std::string mId;
    std::string mType = "ESP32 D1 Mini";
    std::string mVersion = "0.1.0";

    std::string mSoulMate;

    static constexpr size_t sBufferSize = 50;
    char mMessageBuffer[sBufferSize];
    unsigned long mPreviousMillis = 0;
    const unsigned long mInterval = 5000;
    int mCounter = 0;

    ch::Logger log;
};

String ipToString(IPAddress ip) {
    return String{ip[0]} + String{"."} +
        String{ip[1]} + String{"."} +
        String{ip[2]} + String{"."} +
        String{ip[3]};
}

} // namespace ch

#endif
