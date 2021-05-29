#ifndef ARDUINO_BEARSSL_CONFIG_H_
#define ARDUINO_BEARSSL_CONFIG_H_

/* This file is a bit of an unfortunate mess, I wanted to use the same SSL lib for all the boards because it makes supply the certificates in a consistent way.
 * So I went with ArduinoBearSSL as it was the only option at the time with a permissive license. SSLClient is great but is GPLv3.
 * ArduinoBearSSL.h file has the following:
 * 
 * #if defined __has_include
 * #  if __has_include (<ArduinoBearSSLConfig.h>)
 * #    include <ArduinoBearSSLConfig.h>
 * #  endif
 * #endif
 * 
 * That's what we are using this file for, it lets us pass defines into the ArduinoBearSSL library to disable the ECCX08 code.
 * 
 * The location of this file is also important because we have to be able to include it in ESP32Client.h as <ArduinoBearSSLConfig.h> and not "ArduinoBearSSLConfig.h"
 * so it needs to be in a different directory, I know this location one works I suspect other locations would work too as long as it's outside the clients/ dir. 
 */

// Disable ECCX08 on ESP32
#if defined(ESP32)
#define ARDUINO_DISABLE_ECCX08
#endif


// Trying to run more than one SSL stack on the SAMD chips will crush the heap
// However we can configures a small SSL buffer, this issues talks about this
// https://github.com/arduino-libraries/ArduinoBearSSL/pull/12
// It's not really clear to me how low this can go and what the implications are
// It was still working for me at 512 (default is 8192)
#if defined(ARDUINO_ARCH_SAMD)
#define BEAR_SSL_CLIENT_IBUF_SIZE 512 + 85 + 325
#endif

#endif /* ARDUINO_BEARSSL_CONFIG_H_ */