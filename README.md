# ecoh512-wasm
ecoh512-wasm is a WASM implementation of the ECOH 512 bits hashing algorithm, originally created by Daniel R. L. Brown, Matt Campagna, Rene Struik.

This code is based on the [reference implementation](https://web.archive.org/web/20170221001729/http://csrc.nist.gov/groups/ST/hash/sha-3/Round1/documents/ECOH.zip), all rights reserved.
For tests vectors, see "B. Examples" in [their paper](https://ehash.iaik.tugraz.at/uploads/a/a5/Ecoh.pdf).

The JS wrapper for the library and the script `build.sh` is based on [crashdemons](https://github.com/crashdemons/)' work.

## Compiling

To compile this project, use `./build.sh`.
emscripten is required.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[GPLv3](https://www.gnu.org/licenses/gpl-3.0.html)

