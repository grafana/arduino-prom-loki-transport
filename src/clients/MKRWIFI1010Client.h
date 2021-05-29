#ifndef MKRWIFI1010Client_h
#define MKRWIFI1010Client_h

#if defined(ARDUINO_SAMD_MKRWIFI1010)

#include "PLTransport.h"
#include <WiFiNINA.h>
#include <ArduinoBearSSLConfig.h>
#include <ArduinoBearSSL.h>


class MKRWIFI1010Client : public PLTransport {
public:
    MKRWIFI1010Client();
    ~MKRWIFI1010Client();

    //IPromLokiTransport
    bool checkAndReconnectConnection();
    bool disconnect();
    int64_t getTimeMillis();
    Client* getClient();

protected:
    bool _begin();

private:
    Client** _clients = nullptr;
    uint8_t _numClients = 0;
    int _status;
    bool _connected;
    void _connect();
};

#endif // ARDUINO_SAMD_MKRWIFI1010

#endif // MKRWIFI1010Client_h