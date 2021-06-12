#if defined(ESP8266)
#include "ESP8266Client.h"

ESP8266Client::ESP8266Client() {
    _clients = new Client * [_maxClients];
}

ESP8266Client::~ESP8266Client() {
    for (int i = _numClients - 1; i >= 0; i--) {
        delete _clients[i];
    }
    delete[] _clients;
}

bool ESP8266Client::_begin() {
    //TODO error check required fields

    if (_certs || _certLen > 0) {
        errmsg = "SSL is not currently supported on the ESP8266.";
        return false;
    }

    if (_certs && _certLen > 0 && !_useTls) {
        errmsg = "setUseTls(true) was set but no certificates were provided via setCerts. Please provide certificates or setUseTls(false)";
        return false;
    }

    DEBUG_PRINTLN("Connecting Wifi");

    _connect();

    DEBUG_PRINT("Waiting for NTP time sync: ");
    configTime(3 * 3600, 0, _ntpServer);
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(500);
        DEBUG_PRINT(".");
        now = time(nullptr);
    }
    DEBUG_PRINTLN("");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    DEBUG_PRINT("Current time: ");
    DEBUG_PRINTLN(asctime(&timeinfo));

    return true;
}

Client* ESP8266Client::getClient() {

    if (_numClients >= _maxClients) {
        errmsg = "Too many clients requested, increase maxClients";
        return nullptr;
    }


    if (_useTls) {
        //FIXME this won't cleanup properly
        // This works however I can't actually get it to run without crashing
        // looks to me like we are crashing the stack, might be required
        // to use the SPIFF storage for SSL to work around the stack 
        // limitations on the 8266
        // X509List* l = new X509List(_certs);
        // WiFiClientSecure* wifiClient = new WiFiClientSecure();
        // wifiClient->setTrustAnchors(l);
        // _clients[_numClients] = wifiClient;
        // _numClients++;
        errmsg = "SSL not currently supported on ESP8266";
        return nullptr;
    }
    else {
        WiFiClient* wifiClient = new WiFiClient;
        _clients[_numClients] = wifiClient;
        _numClients++;
        return wifiClient;
    }
}

uint16_t ESP8266Client::getConnectCount() {
    return _connectCount;
}

int64_t ESP8266Client::getTimeMillis() {
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    return (uint64_t)tv_now.tv_sec * 1000L + (uint64_t)tv_now.tv_usec / 1000;
};

bool ESP8266Client::checkAndReconnectConnection() {
    // reconnect to WiFi if required
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        yield();
        _connect();
    }
    return true;
};

bool ESP8266Client::disconnect() {
    //FIXME should validate result code here.
    WiFi.disconnect();
    return true;
}

void ESP8266Client::_connect() {
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
