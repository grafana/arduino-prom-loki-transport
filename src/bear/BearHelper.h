#ifndef bearhelper_h
#define bearhelper_h

#include <bearssl/bearssl.h>
#include "brssl.h"




br_x509_trust_anchor* certsToTrustAnchors(char* certs, size_t len, size_t* num);


#endif