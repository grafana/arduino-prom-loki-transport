#if defined(ARDUINO_SAMD_MKRWIFI1010)

#include "MKRWIFI1010Client.h"


MKRWIFI1010Client::MKRWIFI1010Client() {
};

MKRWIFI1010Client::~MKRWIFI1010Client() {
    if (_wifiClient) {
        delete _wifiClient;
    }
    if (_client) {
        delete _client;
    }
};

bool MKRWIFI1010Client::_begin() {
    //TODO error check required fields

    _wifiClient = new WiFiClient();
    if (_useTls) {
        DEBUG_PRINTLN("Using SSL Client");
        const int rand_pin = A5;
        _client = new SSLClient(*_wifiClient, _TAs, _numTAs, rand_pin, 1, SSLClient::SSL_WARN);
    }
    else {
        _client = _wifiClient;
    }

    _connect();
    return true;
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
        return epoch * 1000 * 1000 * 1000;
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
}

bool MKRWIFI1010Client::disconnect() {
    //FIXME should validate result code here.
    WiFi.disconnect();
    return true;
}




#endif