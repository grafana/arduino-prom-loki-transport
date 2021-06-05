# ArduinoPromLokiClient

## Overview

Provides an abstraction over network connectivity for the [GrafanaLoki](https://github.com/grafana/loki-arduino) and [PrometheusArduino](https://github.com/grafana/prometheus-arduino) libraries.

The idea for this library is to encapsulate the work around connecting to a network (Wifi, GSM, etc) and using SSL for connections in a consistent way to make using the Loki and Prometheus libraries easier to use.

The following interface is provided for all the hardware supported by this library:

```
class IPromLokiTransport {
public:
    virtual bool begin() = 0;
    virtual bool checkAndReconnectConnection() = 0;
    virtual bool disconnect() = 0;
    virtual int64_t getTimeMillis() = 0;
    virtual Client* getClient() = 0;
    virtual uint16_t getConnectCount() = 0;

    char* errmsg;
};
```

Note: Client is the Arduino Client interface


## Supported Boards

* ESP32
* MKRWIFI1010
* MKRGSM1400