#include <my_greenpass.h>

#include <base45.h>
#include <base45.c> /* yes.. i don't care */

#include "my_zlib.h"
#include "my_qrscan.h"
#include "my_cose.h"
#include "my_cbor.h"

#include <stdlib.h>
#include <string.h>


struct green_pass {
    cn_cbor *element; // TODO: free me?
    unsigned char *data;
    size_t bsize;
};

GreenPass * gp_new_from_png(const char *fpath) {

    GreenPass *self;
    if (!(self = calloc(1, sizeof(GreenPass))))
        return NULL;

    if (!(self->data = (unsigned char *)qr_scan(fpath))) {
        free(self);
        return NULL;
    }

    self->bsize = strlen((char *)self->data) + 1;
    return self;
}

GreenPass * gp_new_from_str(const char *str) {

    GreenPass *self;
    if (!(self = calloc(1, sizeof(GreenPass))))
        return NULL;

    if (!(self->data = (unsigned char *)strdup(str))) {
        free(self);
        return NULL;
    }

    self->bsize = strlen((char *)self->data) + 1;
    return self;
}

void gp_dump_data_raw(const GreenPass *gp, FILE *ostream) {
    fwrite(gp->data, gp->bsize, 1, ostream);
}

static int gp_decode_b45(GreenPass *gp) {

    unsigned char zbuf[16384] = {0}; // compressed
    size_t zbsize = sizeof zbuf;

    if (strlen((char *)gp->data) < 5)
        return ERR_GP_INVALID_B45;

    // b45 decode, skip the "HC1:" from b45 string
    if (base45_decode(zbuf, &zbsize, (char *)(gp->data + strlen("HC1:")), 0))
        return ERR_GP_INVALID_B45;

    free(gp->data);
    gp->bsize = 0;

    // cpy the decoded buffer
    if (!(gp->data = malloc(zbsize * sizeof(unsigned char))))
        return ERR_GP_MALLOC;

    memcpy(gp->data, zbuf, zbsize);
    gp->bsize = zbsize;
    return ERR_GP_OK;
}

static int gp_decode_zlib(GreenPass *gp) {

    // zlib decompress
    unsigned char *buf;
    size_t bsize = 0;

    if (!(buf = zlib_mem_decompress(gp->data, gp->bsize, &bsize)))
        return ERR_GP_ZLIB_DECOMPRESSION;

    free(gp->data);
    gp->data  = buf;
    gp->bsize = bsize;

    return ERR_GP_OK;
}

static int gp_decode_cose(GreenPass *gp) {

    int err = ERR_GP_OK;
    if ((err = cose_cbor_unserialize(gp->data, gp->bsize, &gp->element)) != ERR_GP_OK)
        return err;

    return ERR_GP_OK;
}

int gp_decode(GreenPass *gp) {

    int err = ERR_GP_OK;

    if ((err = gp_decode_b45(gp)) != ERR_GP_OK)
        goto end;

    if ((err = gp_decode_zlib(gp)) != ERR_GP_OK)
        goto end;

    if ((err = gp_decode_cose(gp)) != ERR_GP_OK)
        goto end;

end:
    return err;
}

void gp_dump(GreenPass *gp, FILE *ostream) {
    cn_cbor_dump(gp->element, ostream, 0, 0);
    fputc('\n', ostream);
}

void gp_destroy(GreenPass **gp) {
    free((*gp)->data);
    free(*gp);
    *gp = NULL;
}