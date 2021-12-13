#include <stddef.h>

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <assert.h>

#if 0
/*
     * This is an ugly hack required to avoid corruption of the input and output data on Windows/MS-DOS systems.
     * Without this,
     * those systems would assume that the input and output files are text, and try to convert
     * the end-of-line characters from one standard to another.
     * That would corrupt binary data, and in particular would render the compressed data unusable.
     * This sets the input and output to binary which suppresses the end-of-line conversions.
     * SET_BINARY_MODE() will be used later on stdin and stdout, at the beginning of main().
     */

    #if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
    #  include <fcntl.h>
    #  include <io.h>
    #  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
    #else
    #  define SET_BINARY_MODE(file)
    #endif

    /* avoid end-of-line conversions */
    SET_BINARY_MODE(file_in);
    SET_BINARY_MODE(file_out);
#endif

// slightly edited version of zerr() https://zlib.net/zlib_how.html
static inline void z_err(FILE *istream, FILE *ostream, int ret) {

    fputs("zpipe: ", stderr);
    switch (ret) {
        case Z_ERRNO:
            if (ferror(istream))
                fputs("error reading istream\n", stderr);
            if (ferror(ostream))
                fputs("error writing ostream\n", stderr);
            break;
        case Z_STREAM_ERROR:
            fputs("invalid compression level\n", stderr);
            break;
        case Z_DATA_ERROR:
            fputs("invalid or incomplete deflate data\n", stderr);
            break;
        case Z_MEM_ERROR:
            fputs("out of memory\n", stderr);
            break;
        case Z_VERSION_ERROR:
            fputs("zlib version mismatch!\n", stderr);
    }

}

// slightly edited? version of inf() https://zlib.net/zlib_how.html
static int zlib_decompress(FILE *source, FILE *dest) {

    static const int Z_CHUNK = 16384;

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[Z_CHUNK];
    unsigned char out[Z_CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {

        strm.avail_in = fread(in, 1, Z_CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }

        if (strm.avail_in == 0) break;

        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {

            strm.avail_out = Z_CHUNK;
            strm.next_out = out;

            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;     /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    return ret;
            }

            have = Z_CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }

        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

// the buffer returned by this function should be free()
unsigned char * zlib_mem_decompress(unsigned char *z_src, size_t z_bsize, size_t *bsize) {

    unsigned char *dst = NULL;

    // zlib decompress
    FILE * file_in = fmemopen(z_src, z_bsize, "r");
    FILE * file_out = open_memstream((char **)&dst, bsize);

    int err = Z_OK;
    if ((err = zlib_decompress(file_in, file_out)) != Z_OK) {

        #ifdef DEBUG
                z_err(file_in, file_out, err);
        #endif

        fclose(file_in), fclose(file_out), free(dst);
        *bsize = 0;
        return NULL;
    }

    fclose(file_in), fclose(file_out);
    return dst;
}
