#pragma once
#include <stdio.h>
#include <cn-cbor/cn-cbor.h>

typedef struct green_pass GreenPass;

enum {
    ERR_GP_OK = 1,
    ERR_GP_MALLOC,
    ERR_GP_INVALID_B45,
    ERR_GP_ZLIB_DECOMPRESSION,

    ERR_GP_COSE_INVALID_SIGN1_OBJECT,
    ERR_GP_COSE_INVALID_SIGN1_ARRAY_WAS_EXPECTED,
    ERR_GP_COSE_INVALID_SIGN1_PAYLOAD_MISSED,
    ERR_GP_COSE_INVALID_SIGN1_PAYLOAD_INVALID,
};

// decode a greenpass from a png
GreenPass * gp_new_from_png(const char *fpath);

// decode a greenpass from a string
GreenPass * gp_new_from_str(const char *str);

// dump the underlining buffer
void gp_dump_data_raw(const GreenPass *gp, FILE *ostream);

int  gp_decode(GreenPass *gp);
void gp_dump(GreenPass *gp, FILE *ostream);

void gp_destroy(GreenPass **gp);