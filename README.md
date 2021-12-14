# greenpass qrcode reader
###### decode & dump data from greenpass 📄
The "evolution" of [greenpass_string_reader](https://github.com/Manu-sh/greenpass_string_reader), less academic approach.

<a href="https://asciinema.org/a/8aGPXfcenT9ue3RWqJ9py0C5Z?autoplay=1&t=00:01">
  <img src="https://asciinema.org/a/8aGPXfcenT9ue3RWqJ9py0C5Z.png" width="320px" height="200px" alt="" />
</a>
<br><br>

Dependencies: [base45-ansi-C](https://github.com/ehn-dcc-development/base45-ansi-C.git), [zlib](https://www.zlib.net/), [COSE-C](https://github.com/cose-wg/COSE-C.git), [zbar](https://github.com/ZBar/ZBar), [libpng](https://github.com/glennrp/libpng).

About **base45-ansi-C** it raise many warning because it make an signed integer overflow (which is **UB**)
~~i don't care for now, but it's important to know~~ i have opened a [pull request](https://github.com/ehn-dcc-development/base45-ansi-C/pull/2) for this.

### Install deps (archlinux)
```bash
pacman -S zbar zlib libpng
```

### Build

```bash
# then into the folder
mkdir -p build && cd build
cmake ..
make -j`nproc`
```

### Useful links

* https://github.com/jojo2234/GreenPass-Experiments
* https://dencode.com/string/base45
* https://lorenzomillucci.it/posts/2021/06/2021-06-28-ubuntu-decodificare-greenpass-covid-19 (italian)
* https://dday.it/redazione/39870/cosa-ce-scritto-nel-qrcode-del-green-pass-come-decodificarlo-per-guardarci-dentro (Italian)
* https://github.com/eu-digital-green-certificates/dgc-testdata

### TODO
* fix leak from exampled copied as is like zbar and so on `valgrind --undef-value-errors=no --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./greenpass_qrcode_reader`

###### Copyright © 2021, [Manu-sh](https://github.com/Manu-sh), s3gmentationfault@gmail.com. Released under the [MIT license](LICENSE).
