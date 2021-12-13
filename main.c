#include <stdio.h>
#include <stdlib.h>
#include <my_greenpass.h>

int main() {

    // example1: decode from a png
    GreenPass *gp = gp_new_from_png("../adolf-gp.png");
    int ec;
    if ((ec = gp_decode(gp)) != ERR_GP_OK) {
        gp_dump_data_raw(gp, stdout);
        gp_destroy(&gp);
        return EXIT_FAILURE;
    }

    gp_dump(gp, stdout);
    gp_destroy(&gp);

    // example2: decode from a string
    gp = gp_new_from_str("HC1:NCFOXNEG2NBJ5*H:QO-.OMBN+XQ99N*6RFS5*TCVWBM*4ODMS0NSRHAL9.4I92P*AVAN9I6T5XH4PIQJAZGA2:UG%U:PI/E2$4JY/KB1TFTJ:0EPLNJ58G/1W-26ALD-I2$VFVVE.80Z0 /KY.SKZC*0K5AFP7T/MV*MNY$N.R6 7P45AHJSP$I/XK$M8TH1PZB*L8/G9HEDCHJ4OIMEDTJCJKDLEDL9CVTAUPIAK29VCN 1UTKFYJZJAPEDI.C$JC7KDF9CFVAPUB1VCSWC%PDMOLHTC$JC3EC66CTS89B9F$8H.OOLI7R3Y+95AF3J6FB5R8QMA70Z37244FKG6T$FJ7CQRB0R%5 47:W0UFJU.UOJ98J93DI+C0UEE-JEJ36VLIWQHH$QIZB%+N+Y2AW2OP6OH6XO9IE5IVU$P26J6 L6/E2US2CZU:80I7JM7JHOJKYJPGK:H3J1D1I3-*TW CXBD+$3PY2C725SS+TDM$SF*SHVT:5D79U+GC5QS+3TAQS:FLU+34IU*9VY-Q9P9SEW-AB+2Q2I56L916CO8T C609O1%NXDU-:R4TICQA.0F2HFLXLLWI8ZU53BMQ2N U:VQQ7RWY91SV2A7N3WQ9J9OAZ00RKLB2");
    if ((ec = gp_decode(gp)) != ERR_GP_OK) {
        gp_dump_data_raw(gp, stdout);
        gp_destroy(&gp);
        return EXIT_FAILURE;
    }

    gp_dump(gp, stdout);
    gp_destroy(&gp);

    return EXIT_SUCCESS;
}

