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

    if (_udp) {
        delete _udp;
    }

    if (_ntpClient) {
        delete _ntpClient;
    }
}

unsigned long getTime() {
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    return (unsigned long)tv_now.tv_sec;
}

bool ESP8266Client::_begin() {
    //TODO error check required fields

    if (_TAs && _numTAs > 0 && !_useTls) {
        errmsg = "setUseTls(true) was set but no certificates were provided via setCerts. Please provide certificates or setUseTls(false)";
        return false;
    }

    // ArduinoBearSSL.onGetTime(getTime);

    DEBUG_PRINTLN("Connecting Wifi");

    _connect();

    _udp = new WiFiUDP();
    _ntpClient = new NTPClient(*_udp, _ntpServer);
    _ntpClient->begin();

    return true;
}

Client* ESP8266Client::getClient() {

    if (_numClients >= _maxClients) {
        errmsg = "Too many clients requested, increase maxClients";
        return nullptr;
    }

    WiFiClient* wifiClient = new WiFiClient;
    _clients[_numClients] = wifiClient;
    _numClients++;
    if (_useTls) {
        // BearSSLClient* sslClient = new BearSSLClient(*wifiClient, _TAs, _numTAs);
        // _clients[_numClients] = sslClient;
        _numClients++;
        // return sslClient;
    }
    else {
        return wifiClient;
    }
}

uint16_t ESP8266Client::getConnectCount() {
    return _connectCount;
}


int64_t ESP8266Client::getTimeMillis() {
    _ntpClient->update();
    uint64_t now = _ntpClient->getEpochTime();
    return now * 1000;
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