#ifndef PLTransport_H
#define PLTransport_H

#include <Arduino.h>
#include <Client.h>
#include <ArduinoBearSSL.h>
#include "PLDebug.h"

class IPromLokiTransport {
public:
    virtual bool begin() = 0;
    virtual bool checkAndReconnectConnection() = 0;
    virtual bool disconnect() = 0;
    virtual int64_t getTimeMillis() = 0;
    virtual Client* getClient() = 0;

    char* errmsg;
};


class PLTransport : public IPromLokiTransport {
public:
    PLTransport(uint8_t maxClients = 10);
    void setUseTls(bool useTls);
    void setCerts(const br_x509_trust_anchor* myTAs, int myNumTAs);
    void setWifiSsid(const char* wifiSsid);
    void setWifiPass(const char* wifiPass);
    void setApn(const char* apn);
    void setApnLogin(const char* apnLogin);
    void setApnPass(const char* apnPass);
    void setNtpServer(char* ntpServer);

    void setDebug(Stream& stream);

    //IPromLokiTransport
    bool begin();
    virtual bool checkAndReconnectConnection() = 0;
    virtual bool disconnect() = 0;
    virtual int64_t getTimeMillis() = 0;
    virtual Client* getClient() = 0;

protected:
    Stream* _debug = nullptr;

    bool _useTls;
    const br_x509_trust_anchor* _TAs;
    uint8_t _numTAs;
    const char* _wifiSsid;
    const char* _wifiPass;
    const char* _apn;
    const char* _apnLogin;
    const char* _apnPass;
    char* _ntpServer = "pool.ntp.org";

    uint8_t _maxClients;

    virtual bool _begin() = 0;

};

#endif