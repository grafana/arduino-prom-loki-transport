#ifndef MKRGSM1400Client_h
#define MKRGSM1400Client_h

#if defined(ARDUINO_SAMD_MKRGSM1400)

#include "PLTransport.h"
#include <MKRGSM.h>
#include <ArduinoBearSSL.h>
#include "bear/BearHelper.h"

class MKRGSM1400Client : public PLTransport
{
public:
    MKRGSM1400Client();
    ~MKRGSM1400Client();

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
    bool _connected;
    void _connect();
    GPRS* _gprs;
    GSM* _gsm;
    GSMClient* _gsmClient;
    uint16_t _connectCount = 0;
};

#endif // ARDUINO_SAMD_MKRGSM1400

#endif // MKRGSM1400Client_h
