// OTAHandler.cpp
// Callum Howard, 2020

#ifndef OTAHANDLER_HPP
#define OTAHANDLER_HPP

#include <WebServer.h>
#include "Utils.hpp"  // Logger

namespace ch {

class OTAHandler {
public:
    OTAHandler(
            const String& hostname,
            const String& ssid,
            const String& password,
            const int port) :
        mHostname{hostname},
        mSsid{ssid},
        mPassword{password},
        mPort{port},
        mServer{port} {
    }

    void begin();
    void update() {
        if (not mInitialised) { return; }
        mServer.handleClient();
    }

private:
    void connectToWifi();
    void beginServing();

    ch::Logger log;
    bool mInitialised = false;
    const String mHostname, mSsid, mPassword, mPort;
    WebServer mServer;

    const char* serverIndex =
        "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
        "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
        "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
        "</form>"
        "<div id='prg'>progress: 0%</div>"
        "<script>"
        "$('form').submit(function(e){"
        "e.preventDefault();"
        "var form = $('#upload_form')[0];"
        "var data = new FormData(form);"
        " $.ajax({"
        "url: '/update',"
        "type: 'POST',"
        "data: data,"
        "contentType: false,"
        "processData:false,"
        "xhr: function() {"
        "var xhr = new window.XMLHttpRequest();"
        "xhr.upload.addEventListener('progress', function(evt) {"
        "if (evt.lengthComputable) {"
        "var per = evt.loaded / evt.total;"
        "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
        "}"
        "}, false);"
        "return xhr;"
        "},"
        "success:function(d, s) {"
        "console.log('success!')"
        "},"
        "error: function (a, b, c) {"
        "}"
        "});"
        "});"
        "</script>";
};

} // namespace ch

#endif
