#ifndef PromLokiTransport_H
#define PromLokiTransport_H

#include "PLTransport.h"

#if defined(ESP32)

#include "clients/ESP32Client.h"
typedef ESP32Client PromLokiTransport;

#elif defined(ESP8266)

#error "ESP8266 is currently unsupported, it's proving too difficult to wokr around the 4k stack size limitation at the moment."

#elif defined(ARDUINO_SAMD_MKRGSM1400)

#include "clients/MKRGSM1400Client.h"
typedef MKRGSM1400Client PromLokiTransport;

#elif defined(ARDUINO_SAMD_MKRWIFI1010)

#include "clients/MKRWIFI1010Client.h"
typedef MKRWIFI1010Client PromLokiTransport;

#else 

#error "Unknown board type, PromLokiTransport does not support this board."

#endif // Types

#endif // PromLokiTransport_H
