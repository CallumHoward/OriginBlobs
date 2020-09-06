// OTAHandler.cpp
// Callum Howard, 2020

#include <ESPmDNS.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
//#include <WiFiClient.h>

#include "OTAHandler.hpp"
#include "Utils.hpp"

namespace ch {

void OTAHandler::begin() {
    connectToWifi();
    beginServing();
    mInitialised = true;
}

void OTAHandler::connectToWifi() {
    // Connect to WiFi network
    WiFi.begin(mSsid.c_str(), mPassword.c_str());
    log.info("\n");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        log.info(".");
    }
    log.info(String("\nConnected to " + mSsid +
            "\nIP address: " + String(WiFi.localIP()) + "\n"));

    /*use mdns for host name resolution*/
    if (!MDNS.begin(mHostname.c_str())) {  // http://esp32.local
        log.info("Error setting up MDNS responder!\n");
        while (1) { delay(1000); }
    }
    log.info("mDNS responder started\n");
}

void OTAHandler::beginServing() {
    const auto rootEndpointHandler = [this]() {
        mServer.sendHeader("Connection", "close");
        mServer.send(200, "text/html", serverIndex);
    };

    const auto updateEndpointHandler = [this]() {
        mServer.sendHeader("Connection", "close");
        mServer.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
    };

    const auto uploadHandler = [this]() {
        HTTPUpload &upload = mServer.upload();
        if (upload.status == UPLOAD_FILE_START) {
            Serial.printf("Update: %s\n", upload.filename.c_str());
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {  // start with max
                                                       // available size
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
            /* flashing firmware to ESP*/
            if (Update.write(upload.buf, upload.currentSize) !=
                    upload.currentSize) {
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_END) {
            if (Update.end(true)) {  // true to set the size to the current
                                     // progress
                Serial.printf(
                        "Update Success: %u\nRebooting...\n", upload.totalSize);
            } else {
                Update.printError(Serial);
            }
        }
    };

    // return index page which is stored in serverIndex
    mServer.on("/", HTTP_GET, rootEndpointHandler);

    // handling uploading firmware file
    mServer.on("/update", HTTP_POST, updateEndpointHandler, uploadHandler);
    mServer.begin();
}

}  // namespace ch
