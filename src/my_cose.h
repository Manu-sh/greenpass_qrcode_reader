#pragma once

#include <my_greenpass.h>
#include <cose/cose.h>

// TODO: free memory etc
static int cose_cbor_unserialize(const unsigned char *buffer, size_t bsize, cn_cbor **element) {

    *element = NULL;
    int type = 0;

    // https://datatracker.ietf.org/doc/html/rfc8152#section-4.2
    HCOSE_SIGN1 cose_sign_1 = (HCOSE_SIGN1)COSE_Decode(buffer, bsize, &type, COSE_sign1_object, NULL);
    if (!cose_sign_1 || type != COSE_sign1_object) {
        // not a sign1 object
        return ERR_GP_COSE_INVALID_SIGN1_OBJECT;
    }

    const cn_cbor *cose_sign_1_decoded = COSE_get_cbor((HCOSE)cose_sign_1);
    if (!cose_sign_1_decoded || cose_sign_1_decoded->type != CN_CBOR_ARRAY) {
        // array was expected
        return ERR_GP_COSE_INVALID_SIGN1_ARRAY_WAS_EXPECTED;
    }

    if (!cose_sign_1_decoded->first_child || !cose_sign_1_decoded->first_child->next || !cose_sign_1_decoded->first_child->next->next) {
        //array[2]: payload was expected
        return ERR_GP_COSE_INVALID_SIGN1_PAYLOAD_MISSED;
    }

    // il 3° element of the array COSE_Sign1 is the payload: https://datatracker.ietf.org/doc/html/rfc8152#section-4.1
    const cn_cbor *payload = cose_sign_1_decoded->first_child->next->next;

    cn_cbor *cbor_payload = cn_cbor_decode((uint8_t *)payload->v.str, payload->length, NULL);
    if (cbor_payload == NULL) {
        // invalid payload
        return ERR_GP_COSE_INVALID_SIGN1_PAYLOAD_INVALID;
    }

    *element = cbor_payload;
    return ERR_GP_OK;
}