# greenpass string reader
###### decode & dump data from greenpass 📄
<a href="https://asciinema.org/a/eRrAulKsa5Yfz7LxL7YuRzzZX?autoplay=1&t=00:02">
  <img src="https://asciinema.org/a/eRrAulKsa5Yfz7LxL7YuRzzZX.png" width="320px" height="200px" alt="" />
</a>
<br><br>

Dependencies: [base45-ansi-C](https://github.com/ehn-dcc-development/base45-ansi-C.git), [zlib](https://www.zlib.net/), [COSE-C](https://github.com/cose-wg/COSE-C.git).

About **base45-ansi-C** it raise many warning because it make an signed integer overflow (which is **UB**)
~~i don't care for now, but it's important to know~~ i have opened a [pull request](https://github.com/ehn-dcc-development/base45-ansi-C/pull/2) for this.

I started this to help a guy, then i have wasted hours trying to figure how to COSE-C should be used to this purpose, 
so i decided to publish here.

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


###### Copyright © 2021, [Manu-sh](https://github.com/Manu-sh), s3gmentationfault@gmail.com. Released under the [MIT license](LICENSE).
