#include "PLTransport.h"

void PLTransport::setUseTls(bool useTls) {
    _useTls = useTls;
}
void PLTransport::setCerts(const br_x509_trust_anchor* myTAs, int myNumTAs) {
    _TAs = myTAs;
    _numTAs = myNumTAs;
}
void PLTransport::setWifiSsid(const char* wifiSsid) {
    _wifiSsid = wifiSsid;
}
void PLTransport::setWifiPass(const char* wifiPass) {
    _wifiPass = wifiPass;
}
void PLTransport::setApn(const char* apn) {
    _apn = apn;
}
void PLTransport::setApnLogin(const char* apnLogin) {
    _apnLogin = apnLogin;
}
void PLTransport::setApnPass(const char* apnPass) {
    _apnPass = apnPass;
}
void PLTransport::setNtpServer(char* ntpServer) {
    _ntpServer = ntpServer;
}

void PLTransport::setDebug(Stream& stream) {
    _debug = &stream;
}

Client* PLTransport::getClient() {
    return _client;
}

bool PLTransport::begin() {
    errmsg = nullptr;
    bool res = _begin();
    if (!res) {
        if (!errmsg) {
            errmsg = "failed to init the client, enable debug logging for more info";
        }
        return false;
    }

    if (!_client) {
        errmsg = "board implementation did not create a Client but also did not error, this is a software bug.";
        return false;
    }
    return true;
};
