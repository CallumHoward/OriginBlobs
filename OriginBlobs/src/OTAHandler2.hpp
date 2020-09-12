// OTAHandler2.cpp
// Callum Howard, 2020

#ifndef OTAHANDLER_HPP
#define OTAHANDLER_HPP

#include <WiFi.h>
#include <Update.h>
#include "Utils.hpp"  // Logger

namespace ch {

// Utility to extract header value from headers
String getHeaderValue(String header, String headerName) {
    return header.substring(strlen(headerName.c_str()));
}

// OTA Logic
void execOTA(String host, int port, String bin) {
    const ch::Logger log{};
    long contentLength = 0;
    bool isValidContentType = false;

    // Wait for connection to establish
    while (WiFi.status() != WL_CONNECTED) {
        log.info("Waiting for Wifi connection\n");
        delay(500);
    }

    WiFiClient client;

    log.info("Connecting to: " + String(host) + "\n");
    // Connect to S3
    if (client.connect(host.c_str(), port)) {
        // Connection Succeed.
        // Fecthing the bin
        log.info("Fetching Bin: " + String(bin + "\n"));

        // Get the contents of the bin file
        client.print(String("GET /") + bin + " HTTP/1.1\r\n" + "Host: " + host +
                "\r\n" + "Cache-Control: no-cache\r\n" +
                "Connection: close\r\n\r\n");

        // Check what is being sent
        //    Serial.print(String("GET ") + bin + " HTTP/1.1\r\n" +
        //                 "Host: " + host + "\r\n" +
        //                 "Cache-Control: no-cache\r\n" +
        //                 "Connection: close\r\n\r\n");

        unsigned long timeout = millis();
        while (client.available() == 0) {
            if (millis() - timeout > 5000) {
                log.info("Client Timeout !\n");
                client.stop();
                return;
            }
        }
        // Once the response is available,
        // check stuff

        /*
           Response Structure
            HTTP/1.1 200 OK
            x-amz-id-2:
           NVKxnU1aIQMmpGKhSwpCBh8y2JPbak18QLIfE+OiUDOos+7UftZKjtCFqrwsGOZRN5Zee0jpTd0=
            x-amz-request-id: 2D56B47560B764EC
            Date: Wed, 14 Jun 2017 03:33:59 GMT
            Last-Modified: Fri, 02 Jun 2017 14:50:11 GMT
            ETag: "d2afebbaaebc38cd669ce36727152af9"
            Accept-Ranges: bytes
            Content-Type: application/octet-stream
            Content-Length: 357280
            Server: AmazonS3

            {{BIN FILE CONTENTS}}

        */
        while (client.available()) {
            // read line till /n
            String line = client.readStringUntil('\n');
            // remove space, to check if the line is end of headers
            line.trim();

            // if the the line is empty,
            // this is end of headers
            // break the while and feed the
            // remaining `client` to the
            // Update.writeStream();
            if (!line.length()) {
                // headers ended
                break;  // and get the OTA started
            }

            // Check if the HTTP Response is 200
            // else break and Exit Update
            if (line.startsWith("HTTP/1.1")) {
                if (line.indexOf("200") < 0) {
                    log.info("Got a non 200 status code from server. "
                                   "Exiting OTA Update.\n");
                    break;
                }
            }

            // extract headers here
            // Start with content length
            if (line.startsWith("Content-Length: ")) {
                contentLength = atol(
                        (getHeaderValue(line, "Content-Length: ")).c_str());
                log.info(
                        "Got " + String(contentLength) + " bytes from server\n");
            }

            // Next, the content type
            if (line.startsWith("Content-Type: ")) {
                String contentType = getHeaderValue(line, "Content-Type: ");
                log.info("Got " + contentType + " payload.\n");
                if (contentType == "application/octet-stream") {
                    isValidContentType = true;
                }
            }
        }
    } else {
        // Connect to S3 failed
        // May be try?
        // Probably a choppy network?
        log.info("Connection to " + String(host) +
                " failed. Please check your setup\n");
        // retry??
        // execOTA();
    }

    // Check what is the contentLength and if content type is
    // `application/octet-stream`
    log.info("contentLength : " + String(contentLength) +
            ", isValidContentType : " + String(isValidContentType) + "\n");

    // check contentLength and content type
    if (contentLength && isValidContentType) {
        // Check if there is enough to OTA Update
        bool canBegin = Update.begin(contentLength);

        // If yes, begin
        if (canBegin) {
            log.info("Begin OTA. This may take 2 - 5 mins to complete. "
                           "Things might be quite for a while.. Patience!\n");
            // No activity would appear on the Serial monitor
            // So be patient. This may take 2 - 5mins to complete
            size_t written = Update.writeStream(client);

            if (written == contentLength) {
                log.info(
                        "Written : " + String(written) + " successfully\n");
            } else {
                log.info("Written only : " + String(written) + "/" +
                        String(contentLength) + ". Retry?\n");
                // retry??
                // execOTA();
            }

            if (Update.end()) {
                log.info("OTA done!\n");
                if (Update.isFinished()) {
                    log.info("Update successfully completed. Rebooting.\n");
                    ESP.restart();
                } else {
                    log.info(
                            "Update not finished? Something went wrong!\n");
                }
            } else {
                log.info("Error Occurred. Error #: " +
                        String(Update.getError()) + "\n");
            }
        } else {
            // not enough space to begin OTA
            // Understand the partitions and
            // space availability
            log.info("Not enough space to begin OTA\n");
            client.flush();
        }
    } else {
        log.info("There was no content in the response\n");
        client.flush();
    }
    ESP.restart();
}

}  // namespace ch

#endif
