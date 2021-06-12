#if defined(ARDUINO_SAMD_MKRGSM1400)

#include "MKRGSM1400Client.h"


MKRGSM1400Client::MKRGSM1400Client() {
    _gprs = new GPRS();
    _gsm = new GSM(false); //true to log AT commands

    _clients = new Client * [_maxClients];
};

MKRGSM1400Client::~MKRGSM1400Client() {
    for (int i = _numClients - 1; i >= 0; i--) {
        delete _clients[i];
    }
    delete[] _clients;
    
    delete _gprs;
    delete _gsm;
    
};

// Keep a global pointer to the instance of the GSM object to use for getting the time into BearSSL
GSM* _globalGSMPointer;
unsigned long getTime() {
    return _globalGSMPointer->getTime();
}

bool MKRGSM1400Client::_begin() {
    //TODO error check required fields

    if (_certs && _certLen > 0 && !_useTls) {
        errmsg = "setUseTls(true) was set but no certificates were provided via setCerts. Please provide certificates or setUseTls(false)";
        return false;
    }
    
    _globalGSMPointer = _gsm;
    ArduinoBearSSL.onGetTime(getTime);

    _connect();

    return true;
};

Client* MKRGSM1400Client::getClient() {
    if (_numClients >= _maxClients) {
        errmsg = "Too many clients requested, increase maxClients";
        return nullptr;
    }

    GSMClient* gsmClient = new GSMClient;
    _clients[_numClients] = gsmClient;
    _numClients++;
    if (_useTls) {
        size_t numTas = 0;
        
        br_x509_trust_anchor* tas = certsToTrustAnchors(_certs, _certLen, &numTas);
        
        BearSSLClient* sslClient = new BearSSLClient(*gsmClient, tas, numTas);
        _clients[_numClients] = sslClient;
        _numClients++;
        return sslClient;
    }
    else {
        return gsmClient;
    }

}

uint16_t MKRGSM1400Client::getConnectCount() {
    return _connectCount;
}

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
            _connectCount++;
        }
        else {
            DEBUG_PRINTLN("Failed to connect to GSM Network");
            delay(2000);
        }
    }
}




#endif
