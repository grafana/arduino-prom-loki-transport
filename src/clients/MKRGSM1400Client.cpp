#if defined(ARDUINO_SAMD_MKRGSM1400)

#include "MKRGSM1400Client.h"


MKRGSM1400Client::MKRGSM1400Client() {
    _gprs = new GPRS();
    _gsm = new GSM(false); //true to log AT commands
};

MKRGSM1400Client::~MKRGSM1400Client() {
    delete _gprs;
    delete _gsm;
    if (_gsmClient) {
        delete _gsmClient;
    }
    if (_client) {
        delete _client;
    }
};

bool MKRGSM1400Client::_begin() {
    //TODO error check required fields

    while (!_connected) {
        DEBUG_PRINTLN("Connecting to GSM Network");
        if ((_gsm->begin("") == GSM_READY) && (_gprs->attachGPRS(_apn, _apnLogin, _apnPass) == GPRS_READY)) {
            _connected = true;
            DEBUG_PRINTLN("GSM Connected Succesfully");
        }
        else {
            DEBUG_PRINTLN("Failed to connect to GSM Network");
            delay(2000);
        }
    }

    if (_useTls) {
        DEBUG_PRINTLN("Using SSL Client");
        _gsmClient = new GSMClient();
        const int rand_pin = A5;
        _client = new SSLClient(*_gsmClient, _TAs, _numTAs, rand_pin, 1, SSLClient::SSL_WARN);
    }
    else {
        _client = new GSMClient();
    }

    return true;

};

int64_t MKRGSM1400Client::getTimeMillis() {
    uint64_t time = _gsm->getTime();
    return time * 1000;
};

bool MKRGSM1400Client::checkAndReconnectConnection() {
    if (!_gsm->isAccessAlive() || _gprs->status() != GPRS_READY) {
        _gprs->detachGPRS();
        _gsm->shutdown();
        _connected = false;
        _connect();
    }
    return true;
}

bool MKRGSM1400Client::disconnect() {
    _gprs->detachGPRS();
    _gsm->shutdown();
    _connected = false;
    return true;
}

void MKRGSM1400Client::_connect() {
    while (!_connected) {
        DEBUG_PRINTLN("Connecting to GSM Network");
        if ((_gsm->begin("") == GSM_READY) && (_gprs->attachGPRS(_apn, _apnLogin, _apnPass) == GPRS_READY)) {
            _connected = true;
            DEBUG_PRINTLN("GSM Connected Succesfully");
        }
        else {
            DEBUG_PRINTLN("Failed to connect to GSM Network");
            delay(2000);
        }
    }
}




#endif