#pragma once
#include <cn-cbor/cn-cbor.h>

// slightly edited version of https://github.com/jimsch/cn-cbor/blob/f713bf67bcf3e076d47e474ce060252ef8be48c7/test/test.c#L42
// note: this is not a real pretty print function, you can't assume that is output can be parsed as json
static inline void dump(const cn_cbor *cb, unsigned indent, bool as_key) {

#define CPY(s, l) fwrite(s, l, 1, stdout)
#define OUT(s) printf("%s", s)
#define PRF(f, a) printf(f, a)

    if (!cb) return;

    unsigned i;
    cn_cbor *cp;
    char finchar = ')'; /* most likely */

    for (i = 0; as_key && i < indent; i++)
        putchar(' ');

    if (!as_key) putchar(' ');

    switch (cb->type) {
        case CN_CBOR_TEXT_CHUNKED:
            OUT("(_\n");
            goto sequence;
        case CN_CBOR_BYTES_CHUNKED:
            OUT("(_\n\n");
            goto sequence;
        case CN_CBOR_TAG:
            PRF("%ld(\n", cb->v.sint);
            goto sequence;
        case CN_CBOR_ARRAY:
            finchar = ']';
            OUT("[\n");
            as_key = true;
            goto sequence;
        case CN_CBOR_MAP:
            finchar = '}';
            OUT("{\n");
            as_key = true;
            goto sequence;
        sequence:
            for (cp = cb->first_child; cp; cp = cp->next, as_key = !as_key) {
                dump(cp, indent+2, as_key);
                if (!as_key) puts(",");
            }

            for (i = 0; as_key && i < indent; i++)
                putchar(' ');

            //if (!as_key) putchar('\n');
            putchar(finchar);
            as_key = false;
            return;
        case CN_CBOR_BYTES:
            OUT("h'");
            for (i = 0; i < cb->length; i++)
                PRF("%02x", cb->v.str[i] & 0xff);
            putchar('\'');
            break;
        case CN_CBOR_TEXT:
            putchar('"');
            CPY(cb->v.str, cb->length); /* should escape stuff */
            putchar('"');
            break;
        case CN_CBOR_NULL:
            OUT("null");
            break;
        case CN_CBOR_TRUE:
            OUT("true");
            break;
        case CN_CBOR_FALSE:
            OUT("false");
            break;
        case CN_CBOR_UNDEF:
            OUT("simple(23)");
            break;
        case CN_CBOR_INT:
            PRF("%ld", cb->v.sint);
            break;
        case CN_CBOR_UINT:
            PRF("%lu", cb->v.uint);
            break;
        case CN_CBOR_DOUBLE:
            PRF("%e", cb->v.dbl);
            break;
        case CN_CBOR_SIMPLE:
            PRF("simple(%ld)", cb->v.sint);
            break;
        default:
            PRF("???%d???", cb->type);
            break;
    }
    if (as_key) putchar(':');
    //if (!as_key) putchar('\n');
}