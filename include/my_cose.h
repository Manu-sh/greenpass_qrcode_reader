#pragma once

#include <cose/cose.h>
#include <my_cbor.h>

static inline void cose_cbor_unserialize(const unsigned char *buffer, size_t bsize) {

    int type = 0;

    // https://datatracker.ietf.org/doc/html/rfc8152#section-4.2
    HCOSE_SIGN1 cose_sign_1 = (HCOSE_SIGN1)COSE_Decode(buffer, bsize, &type, COSE_sign1_object, NULL);
    if (!cose_sign_1 || type != COSE_sign1_object) {
        fprintf(stderr, "not a sign1 object\n");
        return;
    }

    const cn_cbor *cose_sign_1_decoded = COSE_get_cbor((HCOSE)cose_sign_1);
    if (!cose_sign_1_decoded || cose_sign_1_decoded->type != CN_CBOR_ARRAY) {
        fprintf(stderr, "code_sign_1: array was expected\n");
        return;
    }

    if (!cose_sign_1_decoded->first_child || !cose_sign_1_decoded->first_child->next || !cose_sign_1_decoded->first_child->next->next) {
        fprintf(stderr, "array[2]: payload was expected\n");
        return;
    }

    // il 3° element of the array COSE_Sign1 is the payload: https://datatracker.ietf.org/doc/html/rfc8152#section-4.1
    const cn_cbor *payload = cose_sign_1_decoded->first_child->next->next;

    const cn_cbor *cbor_payload = cn_cbor_decode((uint8_t *)payload->v.str, payload->length, NULL);
    if (cbor_payload == NULL) {
        fprintf(stderr, "invalid payload\n");
        return;
    }

    dump(cbor_payload, 0, 0);
    puts("");
}