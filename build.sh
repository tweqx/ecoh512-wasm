#!/bin/bash

# emscripten binaries need to be in your $PATH, run "source ./emsdk_env.sh" in the emscripten installation directory to do that

emcc ecoh512-wasm.c ecoh.param.c ecoh571.c -O3 -o dist/ecoh512.js -s MODULARIZE=1 -s 'EXPORT_NAME="createECOH512Module"' -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -s EXPORTED_FUNCTIONS="['_malloc', '_free']" -s WASM=1

if [ $? == 0 ]; then
  cat dist/ecoh512.js wrapper/wrapper.js > dist/ecoh512-wasm.js ;
  rm dist/ecoh512.js
fi

