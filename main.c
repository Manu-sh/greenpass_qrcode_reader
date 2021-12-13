#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <my_greenpass.h>

int main(int argc, char *argv[]) {

    GreenPass *gp = gp_new_from_png("/home/user/Documenti/gp1.png");
    gp_dump_data_raw(gp, stdout);

    assert(gp_decode_b45(gp) == ERR_GP_OK);
    assert(gp_decode_zlib(gp) == ERR_GP_OK);

    // CBOR/COSE data decode
    cose_cbor_unserialize(gp->data, gp->bsize);
    return EXIT_SUCCESS;
}

