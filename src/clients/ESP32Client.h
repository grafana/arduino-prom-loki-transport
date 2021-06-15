#ifndef ESP32Client_h
#define ESP32Client_h

#if defined(ESP32)

#include "PLTransport.h"
#include <WiFi.h>
#include <time.h>
#include <esp_sntp.h>
#include <ArduinoBearSSL.h>
#include "bear/BearHelper.h"

class ESP32Client : public PLTransport {
public:
    ESP32Client();
    ~ESP32Client();

    //IPromLokiTransport
    bool checkAndReconnectConnection();
    bool disconnect();
    int64_t getTimeMillis();
    Client* getClient();
    uint16_t getConnectCount();

protected:
    bool _begin();


private:
    Client** _clients = nullptr;
    uint8_t _numClients = 0;
    void _connect();
    uint16_t _connectCount = 0;
};

#endif // ESP32

#endif // ESP32Client_h
