#ifndef ESP8266Client_h
#define ESP8266Client_h

#if defined(ESP8266)

#include "PLTransport.h"
#include <ESP8266WiFi.h>
#include <time.h>

class ESP8266Client : public PLTransport
{
public:
    ESP8266Client();
    ~ESP8266Client();

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

#endif // ESP8266

#endif // ESP8266Client_h
