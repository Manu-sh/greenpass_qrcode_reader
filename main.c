#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <my_greenpass.h>

int main(int argc, char *argv[]) {

    GreenPass *gp = gp_new_from_png("../adolf-gp.png");
    gp_dump_data_raw(gp, stdout);

    assert(gp_decode_b45(gp) == ERR_GP_OK);
    // gp_dump_data_raw(gp, stdout);

    assert(gp_decode_zlib(gp) == ERR_GP_OK);
    // gp_dump_data_raw(gp, stdout);

    // CBOR/COSE data decode
    cose_cbor_unserialize(gp->data, gp->bsize); // TODO: refactor this

    gp_destroy(&gp);
    return EXIT_SUCCESS;
}

