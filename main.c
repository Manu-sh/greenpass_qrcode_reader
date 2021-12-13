#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <base45.h>
#include <base45.c> /* yes.. i don't care */

#include <my_zlib.h>
#include <my_cbor.h>

#include <cose/cose.h>


static inline void cbor_unserialize(const unsigned char *buffer, size_t bsize) {

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

int main(int argc, char *argv[]) {

    // https://github.com/jojo2234/GreenPass-Experiments
    const char *in = "NCFOXNEG2NBJ5*H:QO-.OMBN+XQ99N*6RFS5*TCVWBM*4ODMS0NSRHAL9.4I92P*AVAN9I6T5XH4PIQJAZGA2:UG%U:PI/E2$4JY/KB1TFTJ:0EPLNJ58G/1W-26ALD-I2$VFVVE.80Z0 /KY.SKZC*0K5AFP7T/MV*MNY$N.R6 7P45AHJSP$I/XK$M8TH1PZB*L8/G9HEDCHJ4OIMEDTJCJKDLEDL9CVTAUPIAK29VCN 1UTKFYJZJAPEDI.C$JC7KDF9CFVAPUB1VCSWC%PDMOLHTC$JC3EC66CTS89B9F$8H.OOLI7R3Y+95AF3J6FB5R8QMA70Z37244FKG6T$FJ7CQRB0R%5 47:W0UFJU.UOJ98J93DI+C0UEE-JEJ36VLIWQHH$QIZB%+N+Y2AW2OP6OH6XO9IE5IVU$P26J6 L6/E2US2CZU:80I7JM7JHOJKYJPGK:H3J1D1I3-*TW CXBD+$3PY2C725SS+TDM$SF*SHVT:5D79U+GC5QS+3TAQS:FLU+34IU*9VY-Q9P9SEW-AB+2Q2I56L916CO8T C609O1%NXDU-:R4TICQA.0F2HFLXLLWI8ZU53BMQ2N U:VQQ7RWY91SV2A7N3WQ9J9OAZ00RKLB2";

    unsigned char zbuf[16384] = {0}; // compressed
    size_t zbsize = sizeof zbuf;

    // b45 decode
    if (base45_decode(zbuf, &zbsize, in, 0)) {
        fprintf(stderr, "invalid base45 string\n");
        return EXIT_FAILURE;
    }

    // zlib decompress
    unsigned char *buf;
    size_t bsize = 0;
    if (!(buf = zlib_mem_decompress(zbuf, zbsize, &bsize)))
        return EXIT_FAILURE;


    // CBOR/COSE data decode
    cbor_unserialize(buf, bsize);

    free(buf);
    return EXIT_SUCCESS;
}

