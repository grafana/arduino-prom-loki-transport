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

## Not Supported Boards :(

* ESP8266: I've tried to make this work but it seems really finicky because of the 4k stack limitation, in some circumstances it can work but not with TLS support and the Prometheus library has a hard time as well because the Prometheus proto has several layers of nested objects which leads to a lot of function calls and blowing up the stack.  I don't really think it's going to be worth the time to try to get this to work but I'm not gonna rip out the code and work done so far.

## Licensing Note

This library directly includes code from the [BearSSL](https://bearssl.org/) project in the `src/bear` directory, the files included from that project are available under the MIT license and maintain the copyright of the original author. They are mostly included unchanged with a few exceptions such as renaming `xmalloc` to `ymalloc` to avoid conflicts with an existing `xmalloc` function in another library.