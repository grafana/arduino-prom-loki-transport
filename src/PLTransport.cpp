#include "PLTransport.h"

PLTransport::PLTransport(uint8_t maxClients) : _maxClients(maxClients){};

void PLTransport::setUseTls(bool useTls) {
    _useTls = useTls;
};
void PLTransport::setCerts(const char* certs, size_t len) {
    _certs = certs;
    _certLen = len;
};
void PLTransport::setWifiSsid(const char* wifiSsid) {
    _wifiSsid = wifiSsid;
};
void PLTransport::setWifiPass(const char* wifiPass) {
    _wifiPass = wifiPass;
};
void PLTransport::setApn(const char* apn) {
    _apn = apn;
};
void PLTransport::setApnLogin(const char* apnLogin) {
    _apnLogin = apnLogin;
};
void PLTransport::setApnPass(const char* apnPass) {
    _apnPass = apnPass;
};
void PLTransport::setNtpServer(char* ntpServer) {
    _ntpServer = ntpServer;
};

void PLTransport::setDebug(Stream& stream) {
    _debug = &stream;
};


bool PLTransport::begin() {
    errmsg = nullptr;
    bool res = _begin();
    if (!res) {
        if (!errmsg) {
            errmsg = (char*)"failed to init the client, enable debug logging for more info";
        }
        return false;
    }
    return true;
};
