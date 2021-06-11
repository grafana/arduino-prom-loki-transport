# PromLokiTransport

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

## Dependencies

* **ArduinoBearSSL** Needed for all supported boards
* **ArduinoECCX08** Only needed for MKR boards, however might still be needed to compile for others
* **MKRGSM** Only needed for MKRGSM1400
* **WiFiNINA** Only needed for the MKRWIFI1010

## Supported Boards

* ESP32
* MKRWIFI1010
* MKRGSM1400

## Not Supported Boards :(

* ESP8266: I've tried to make this work but it seems really finicky because of the 4k stack limitation, in some circumstances it can work but not with TLS support and the Prometheus library has a hard time as well because the Prometheus proto has several layers of nested objects which leads to a lot of function calls and blowing up the stack.  I don't really think it's going to be worth the time to try to get this to work but I'm not gonna rip out the code and work done so far.

## SSL Note

In an attempt to provide the "best" SSL experience this library uses the ArduinoBearSSL lib for SSL whereever possible rather than using some of the underlying SSL implementations provided with the board software.

### Providing CA certs for servers

PEM formatted certs will likely be the easiest way to provide a CA cert

Look at the examples in [GrafanaLoki](https://github.com/grafana/loki-arduino) and [PrometheusArduino](https://github.com/grafana/prometheus-arduino) libraries.

There are several ways to obtain the PEM certificate for a server, including directly from a browser I will try to circle back around here in the future and add examples.

The BearSSL tooling used to parse the cert also understands DER formatted binary certs too however I have never tested this myself.

### Mutual TLS

This hasn't been wired up to this libary yet but should be possible, it's supported by BearSSL

### MKR devices and Buffers

The MKR devices are really at the low end for available RAM and SSL support but it does work. I did find however that when trying to run a Loki and Prometheus client on the same device that creating these 2 SSL connections would crush the RAM and cause failures, I dug into this and found that ArduionBearSSL configures a default input buffer for SSL at something like 8kb, so 2 SSL connections immediately consumes 16kb of valuable RAM.

It turns out you can tune this and if you look in `ArduinoBearSSLConfig.h` we do just that for the SAMD devices like the MKR's. 

This feels a little brittle however, as it's possible a server may not support the TLS frgament extension and will send more data than fits in the buffer leading most likely to failures/crashes or who knows what.

Neither Loki or Prometheus really return much in terms of large payloads so I think this should hopefully work out fine even on servers that don't understand the TLS fragment extension but I didn't have one to test so... caveat emptor.


## Licensing Note

This library directly includes code from the [BearSSL](https://bearssl.org/) project in the `src/bear` directory, the files included from that project are available under the MIT license and maintain the copyright of the original author. They are mostly included unchanged with a few exceptions such as renaming `xmalloc` to `ymalloc` to avoid conflicts with an existing `xmalloc` function in another library.