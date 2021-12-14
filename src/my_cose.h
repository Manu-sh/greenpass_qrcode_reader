#pragma once

#include <my_greenpass.h>
#include <cose/cose.h>

// cn_cbor_clone() https://github.com/cose-wg/COSE-C/blob/97d1805e71b7a6770093c5e6790d46611680d563/include/cose/cose.h#L808

typedef struct {
    HCOSE_SIGN1 cose_sign_1;
    cn_cbor *cbor_payload;
    int gp_error;
} COSE_Wrap;

static inline COSE_Wrap * cose_cbor_unserialize(const unsigned char *buffer, size_t bsize) {

    COSE_Wrap *wrap = calloc(1, sizeof(COSE_Wrap));
    assert(wrap);

    // set no error
    wrap->gp_error = ERR_GP_OK;

    int type = 0;

    // https://datatracker.ietf.org/doc/html/rfc8152#section-4.2
    HCOSE_SIGN1 cose_sign_1 = (HCOSE_SIGN1)COSE_Decode(buffer, bsize, &type, COSE_sign1_object, NULL);
    if (!cose_sign_1 || type != COSE_sign1_object) {
        // not a sign1 object
        COSE_Sign1_Free(cose_sign_1);
        wrap->gp_error = ERR_GP_COSE_INVALID_SIGN1_OBJECT;
        return wrap;
    }

    const cn_cbor *cose_sign_1_decoded = COSE_get_cbor((HCOSE)cose_sign_1);
    if (!cose_sign_1_decoded || cose_sign_1_decoded->type != CN_CBOR_ARRAY) {
        // array was expected
        COSE_Sign1_Free(cose_sign_1);
        wrap->gp_error = ERR_GP_COSE_INVALID_SIGN1_ARRAY_WAS_EXPECTED;
        return wrap;
    }

    if (!cose_sign_1_decoded->first_child || !cose_sign_1_decoded->first_child->next || !cose_sign_1_decoded->first_child->next->next) {
        //array[2]: payload was expected
        COSE_Sign1_Free(cose_sign_1);
        wrap->gp_error = ERR_GP_COSE_INVALID_SIGN1_PAYLOAD_MISSED;
        return wrap;
    }

    // il 3° element of the array COSE_Sign1 is the payload: https://datatracker.ietf.org/doc/html/rfc8152#section-4.1
    const cn_cbor *payload = cose_sign_1_decoded->first_child->next->next;

    cn_cbor *cbor_payload = cn_cbor_decode((uint8_t *)payload->v.str, payload->length, NULL);
    if (cbor_payload == NULL) {
        // invalid payload
        COSE_Sign1_Free(cose_sign_1);
        wrap->gp_error = ERR_GP_COSE_INVALID_SIGN1_PAYLOAD_INVALID;
        return wrap;
    }

    wrap->cose_sign_1  = cose_sign_1;
    wrap->cbor_payload = cbor_payload;
    wrap->gp_error     = ERR_GP_OK;

    return wrap;
}

static inline int cose_cbor_get_error(COSE_Wrap *wrap) {
    return wrap->gp_error;
}

static inline bool cose_cbor_has_error(COSE_Wrap *wrap) {
    return cose_cbor_get_error(wrap) != ERR_GP_OK;
}

static inline cn_cbor * cose_cbor_unwrap(COSE_Wrap *wrap) {
    return wrap->cbor_payload;
}

static inline void cose_cbor_destroy(COSE_Wrap **wrap) {

    if (!wrap) return;

    if ((*wrap)->cose_sign_1)
        COSE_Sign1_Free((*wrap)->cose_sign_1);

    (*wrap)->cose_sign_1  = NULL;
    (*wrap)->cbor_payload = NULL;

    free(*wrap);
    *wrap = NULL;
}