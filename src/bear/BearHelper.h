#ifndef bearhelper_h
#define bearhelper_h

#if !defined(ESP8266)

#ifdef __cplusplus
extern "C" {
#endif

#include "brssl.h"

br_x509_trust_anchor* certsToTrustAnchors(const char* certs, size_t len, size_t* num);

#ifdef __cplusplus
}
#endif

#endif // ESP8266


#endif
