#include <emscripten/emscripten.h>
#include <stdlib.h>

#define _x64_
#include "ecoh.h"

EMSCRIPTEN_KEEPALIVE
hashState* ecoh512_init(void) {
  hashState* state = malloc(sizeof(hashState));
  if (state == NULL)
    return NULL;

  Init(state, 512);

  return state;
}

EMSCRIPTEN_KEEPALIVE
void ecoh512_update(hashState* state, const unsigned char *data, size_t len) {
  if (state == NULL)
    return;

  Update(state, data, len);
}

EMSCRIPTEN_KEEPALIVE
void ecoh512_final(hashState* state, unsigned char* digest) {
  if (state == NULL)
    return;

  Final(state, digest);
}

EMSCRIPTEN_KEEPALIVE
void ecoh512_cleanup(hashState* state) {
  if (state == NULL)
    return;

  free(state);
}

