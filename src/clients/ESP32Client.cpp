#if defined(ESP32)
#include "ESP32Client.h"

ESP32Client::ESP32Client() {
    _clients = new Client * [_maxClients];
}

ESP32Client::~ESP32Client() {
    for (int i = _numClients - 1; i >= 0; i--) {
        delete _clients[i];
    }
    delete[] _clients;
}

unsigned long getTime() {
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    return (unsigned long)tv_now.tv_sec;
}

bool ESP32Client::_begin() {
    //TODO error check required fields

    if (_certs && _certLen > 0 && !_useTls) {
        errmsg = (char*)"setUseTls(true) was set but no certificates were provided via setCerts. Please provide certificates or setUseTls(false)";
        return false;
    }

    ArduinoBearSSL.onGetTime(getTime);

    DEBUG_PRINTLN("Connecting Wifi");

    _connect();

    DEBUG_PRINT("Setting up sntp and setting time from: ");
    DEBUG_PRINTLN(_ntpServer);

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, _ntpServer);
    sntp_init();

    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET) {
        delay(1000);
        DEBUG_PRINT(".");
    }

    DEBUG_PRINTLN("Time set succesfully");

    return true;
}

Client* ESP32Client::getClient() {

    if (_numClients >= _maxClients) {
        errmsg = (char*)"Too many clients requested, increase maxClients";
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

uint16_t ESP32Client::getConnectCount() {
    return _connectCount;
}


int64_t ESP32Client::getTimeMillis() {
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    return (uint64_t)tv_now.tv_sec * 1000L + (uint64_t)tv_now.tv_usec / 1000;
};

bool ESP32Client::checkAndReconnectConnection() {
    // reconnect to WiFi if required
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        yield();
        _connect();
    }
    return true;
};

bool ESP32Client::disconnect() {
    //FIXME should validate result code here.
    WiFi.disconnect();
    return true;
}

void ESP32Client::_connect() {
    DEBUG_PRINT("Connecting to '");
    DEBUG_PRINT(_wifiSsid);
    DEBUG_PRINT("' ...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(_wifiSsid, _wifiPass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        DEBUG_PRINT(".");
    }
    DEBUG_PRINTLN("connected");

    DEBUG_PRINT("IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
    _connectCount++;
}


#endif
