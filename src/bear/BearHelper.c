
#if !defined(ESP8266)

#include "BearHelper.h"

int
looks_like_DER(const unsigned char* buf, size_t len)
{
    int fb;
    size_t dlen;

    if (len < 2) {
        return 0;
    }
    if (*buf++ != 0x30) {
        return 0;
    }
    fb = *buf++;
    len -= 2;
    if (fb < 0x80) {
        return (size_t)fb == len;
    }
    else if (fb == 0x80) {
        return 0;
    }
    else {
        fb -= 0x80;
        if (len < (size_t)fb + 2) {
            return 0;
        }
        len -= (size_t)fb;
        dlen = 0;
        while (fb-- > 0) {
            if (dlen > (len >> 8)) {
                return 0;
            }
            dlen = (dlen << 8) + (size_t)*buf++;
        }
        return dlen == len;
    }
}

static void
vblob_append(void* cc, const void* data, size_t len)
{
    bvector* bv;

    bv = cc;
    VEC_ADDMANY(*bv, data, len);
}

pem_object*
decode_pem(const void* src, size_t len, size_t* num)
{
    VECTOR(pem_object) pem_list = VEC_INIT;
    br_pem_decoder_context pc;
    pem_object po, * pos;
    const unsigned char* buf;
    bvector bv = VEC_INIT;
    int inobj;
    int extra_nl;

    *num = 0;
    br_pem_decoder_init(&pc);
    buf = src;
    inobj = 0;
    po.name = NULL;
    po.data = NULL;
    po.data_len = 0;
    extra_nl = 1;
    while (len > 0) {
        size_t tlen;

        tlen = br_pem_decoder_push(&pc, buf, len);
        buf += tlen;
        len -= tlen;
        switch (br_pem_decoder_event(&pc)) {

        case BR_PEM_BEGIN_OBJ:
            po.name = xstrdup(br_pem_decoder_name(&pc));
            br_pem_decoder_setdest(&pc, vblob_append, &bv);
            inobj = 1;
            break;

        case BR_PEM_END_OBJ:
            if (inobj) {
                po.data = VEC_TOARRAY(bv);
                po.data_len = VEC_LEN(bv);
                VEC_ADD(pem_list, po);
                VEC_CLEAR(bv);
                po.name = NULL;
                po.data = NULL;
                po.data_len = 0;
                inobj = 0;
            }
            break;

        case BR_PEM_ERROR:
            xfree(po.name);
            VEC_CLEAR(bv);
            fprintf(stderr,
                "ERROR: invalid PEM encoding\n");
            VEC_CLEAREXT(pem_list, &free_pem_object_contents);
            return NULL;
        }

        /*
         * We add an extra newline at the end, in order to
         * support PEM files that lack the newline on their last
         * line (this is somwehat invalid, but PEM format is not
         * standardised and such files do exist in the wild, so
         * we'd better accept them).
         */
        if (len == 0 && extra_nl) {
            extra_nl = 0;
            buf = (const unsigned char*)"\n";
            len = 1;
        }
    }
    if (inobj) {
        fprintf(stderr, "ERROR: unfinished PEM object\n");
        xfree(po.name);
        VEC_CLEAR(bv);
        VEC_CLEAREXT(pem_list, &free_pem_object_contents);
        return NULL;
    }

    *num = VEC_LEN(pem_list);
    VEC_ADD(pem_list, po);
    pos = VEC_TOARRAY(pem_list);
    VEC_CLEAR(pem_list);
    return pos;
}

void
free_pem_object_contents(pem_object* po)
{
    if (po != NULL) {
        xfree(po->name);
        xfree(po->data);
    }
}


br_x509_trust_anchor* certsToTrustAnchors(const char* certs, size_t len, size_t* num) {

    VECTOR(br_x509_certificate) cert_list = VEC_INIT;
    pem_object* pos;
    size_t u, num_pos;
    br_x509_certificate* xcs;
    br_x509_certificate dummy;

    *num = 0;

    /*
         * Check for a DER-encoded certificate.
         */
    if (looks_like_DER(certs, len)) {
        xcs = ymalloc(2 * sizeof * xcs);
        xcs[0].data = (unsigned char*)certs;
        xcs[0].data_len = len;
        xcs[1].data = NULL;
        xcs[1].data_len = 0;
        *num = 1;
        //FIXME
        return NULL;
    }

    pos = decode_pem(certs, len, &num_pos);
    // xfree(buf);
    if (pos == NULL) {
        return NULL;
    }
    for (u = 0; u < num_pos; u++) {
        if (eqstr(pos[u].name, "CERTIFICATE")
            || eqstr(pos[u].name, "X509 CERTIFICATE"))
        {
            br_x509_certificate xc;

            xc.data = pos[u].data;
            xc.data_len = pos[u].data_len;
            pos[u].data = NULL;
            VEC_ADD(cert_list, xc);
        }
    }
    for (u = 0; u < num_pos; u++) {
        free_pem_object_contents(&pos[u]);
    }
    xfree(pos);

    if (VEC_LEN(cert_list) == 0) {
        fprintf(stderr, "ERROR: no certificates in string\n");
        return NULL;
    }
    *num = VEC_LEN(cert_list);
    dummy.data = NULL;
    dummy.data_len = 0;
    VEC_ADD(cert_list, dummy);
    xcs = VEC_TOARRAY(cert_list);
    VEC_CLEAR(cert_list);

    br_x509_trust_anchor* tas;
    tas = ymalloc(*num * sizeof * tas);

    for (int i = 0; i < *num; i++) {
        tas[i] = *certificate_to_trust_anchor(&xcs[i]);
    }

    return tas;

}

#endif // ESP8266
