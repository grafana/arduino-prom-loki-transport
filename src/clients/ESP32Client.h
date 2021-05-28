#ifndef ESP32Client_h
#define ESP32Client_h

#if defined(ESP32)

#include "PLTransport.h"
#include <WiFi.h>
#include <time.h>
#include <esp_sntp.h>
#include <SSLClient.h>

class ESP32Client : public PLTransport
{
public:
    ESP32Client();
    ~ESP32Client();

    //IPromLokiTransport
    bool checkAndReconnectConnection();
    bool disconnect();
    int64_t getTimeMillis();

protected:
    bool _begin();
    

private:
    WiFiClient *_wifiClient;
    void _connect();
};

#endif // ESP32

#endif // ESP32Client_h