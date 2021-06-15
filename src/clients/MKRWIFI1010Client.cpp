#if defined(ARDUINO_SAMD_MKRWIFI1010)

#include "MKRWIFI1010Client.h"

// Need a static member function to use for BearSSL's getTime.
unsigned long getTime() {
    return WiFi.getTime();
}

MKRWIFI1010Client::MKRWIFI1010Client() {
    _clients = new Client * [_maxClients];
};

MKRWIFI1010Client::~MKRWIFI1010Client() {
    for (int i = _numClients - 1; i >= 0; i--) {
        delete _clients[i];
    }
    delete[] _clients;
};

bool MKRWIFI1010Client::_begin() {
    //TODO error check required fields

    if (_certs && _certLen > 0 && !_useTls) {
        errmsg = "setUseTls(true) was set but no certificates were provided via setCerts. Please provide certificates or setUseTls(false)";
        return false;
    }

    ArduinoBearSSL.onGetTime(getTime);

    _connect();

    return true;
};

Client* MKRWIFI1010Client::getClient() {

    if (_numClients >= _maxClients) {
        errmsg = "Too many clients requested, increase maxClients";
        return nullptr;
    }

    WiFiClient* wifiClient = new WiFiClient;
    _clients[_numClients] = wifiClient;
    _numClients++;
    if (_useTls) {
        size_t numTas = 0;
        
        br_x509_trust_anchor* tas = certsToTrustAnchors(_certs, _certLen, &numTas);
        
        BearSSLClient* sslClient = new BearSSLClient(*wifiClient, tas, numTas);
        _clients[_numClients] = sslClient;
        _numClients++;
        return sslClient;
    }
    else {
        return wifiClient;
    }
}

uint16_t MKRWIFI1010Client::getConnectCount(){
    return _connectCount;
};

int64_t MKRWIFI1010Client::getTimeMillis() {
    //FIXME
    uint64_t epoch;
    int numberOfTries = 0, maxTries = 6;
    do {
        epoch = WiFi.getTime();
        numberOfTries++;
        if (epoch == 0) {
            delay(2000);
        }
    } while ((epoch == 0) && (numberOfTries < maxTries));

    if (numberOfTries >= maxTries) {
        DEBUG_PRINTLN("NTP unreachable!!");
        return 0;
    }
    else {
        return epoch * 1000;
    }
};

bool MKRWIFI1010Client::checkAndReconnectConnection() {
    //reconnect to WiFi if required
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        _connect();
    }
    return true;
}

void MKRWIFI1010Client::_connect() {
    while (_status != WL_CONNECTED) {

        DEBUG_PRINT("Attempting to connect to SSID: ");
        DEBUG_PRINTLN(_wifiSsid);

        _status = WiFi.begin(_wifiSsid, _wifiPass);
        // wait 5 seconds for connection:
        delay(5000);
    }

    DEBUG_PRINTLN("Connected to wifi");
    DEBUG_PRINT("IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
    _connectCount++;
}

bool MKRWIFI1010Client::disconnect() {
    //FIXME should validate result code here.
    WiFi.disconnect();
    return true;
}




#endif
