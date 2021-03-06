/* don't remove this line */
if (typeof createECOH512Module === 'undefined') {
  createECOH512Module = Promise.reject(new Error('ecoh512 wasm module was not available'));
}

var ecoh512 = {
  internal: {
    module: null,
    bytesFromBuffer: function(internalBuffer, bufLen) {
      const resultView = new Uint8Array(this.module.HEAP8.buffer, internalBuffer, bufLen); // view, not a copy
      const result = new Uint8Array(resultView); // copy, not a view!
      return result;
    },

    bufferFromBytes: function(bytes) {
      var internalBuffer = this.create_buffer(bytes.length);
      this.applyBytesToBuffer(bytes, internalBuffer);
      return internalBuffer;
    },
    applyBytesToBuffer: function(bytes, internalBuffer) {
      this.module.HEAP8.set(bytes, internalBuffer);
    },
    toHex: function(bytes) {
      return Array.prototype.map.call(bytes, function(n) {
        return (n < 16 ? '0' : '') + n.toString(16)
      }).join('');
    },
    inputToBytes: function (input) {
      if (input instanceof Uint8Array)
        return input;
      else if (typeof input === 'string')
        return (new TextEncoder()).encode(input);
      else
        throw new Error('Input must be an string, Buffer or Uint8Array');
    }
  },

  /**
   * Checks if ECOH512 support is ready (WASM Module loaded)
   * @return {Boolean}
   */
  isReady: function() {
    return ecoh512.internal.module !== null;
  },

  /**
   * Initializes a Hashing Context for Hash
   * @return {Object} the context object for this hashing session. Should only be used to hash one data source.
   */
  init: function() {
    return {
      'digest_size': 512,
      'context': ecoh512.internal.init()
    };
  },

  /**
   * Update the hashing context with new input data
   * @param {Object} contextObject the context object for this hashing session
   * @param {Uint8Array} bytes an array of bytes to hash
   */
  update: function(contextObject, bytes) {
    var inputBuffer = ecoh512.internal.bufferFromBytes(bytes);

    ecoh512.internal.update(contextObject.context, inputBuffer, bytes.length * 8);

    ecoh512.internal.destroy_buffer(inputBuffer);
  },

  /**
   * Update the hashing context with new input data
   * @param {Object} contextObject the context object for this hashing session
   * @param {Object} value the value to use as bytes to update the hash calculation. Must be String or Uint8Array.
   */
   updateFromValue: function(contextObject, value) {
     ecoh512.update(contextObject, ecoh512.internal.inputToBytes(value));
   },

  /**
   * Finalizes the hashing session and produces digest ("hash") bytes.
   * Size of the returned array is always digest_size/8 bytes long.
   * This method does not clean up the hashing context - be sure to call cleanup(ctx) !
   * @param {Object} contextObject the context object for this hashing session
   * @return {Uint8Array} an array of bytes representing the raw digest ("hash") value.
   */
  final: function(contextObject) {
    var digestByteLen = contextObject.digest_size / 8;
    var digestBuffer = ecoh512.internal.create_buffer(digestByteLen);

    ecoh512.internal.final(contextObject.context, digestBuffer);

    var digestBytes = ecoh512.internal.bytesFromBuffer(digestBuffer, digestByteLen);
    ecoh512.internal.destroy_buffer(digestBuffer);
    return digestBytes;
  },

  /**
   * Cleans up and releases the Context object for the (now ended) hashing session.
   * @param {Object} contextObject the context object for this hashing session
   */
  cleanup: function(contextObject) {
    ecoh512.internal.cleanup(contextObject.context);
  },

  /**
   * Calculates the ecoh512 message digest ("hash") for the input bytes or string
   * @param {Object} input the input value to hash - either Uint8Array or String
   * @param {Number} digest_size the number of bits for the digest size. 512 is default.
   * @return {Uint8Array} an array of bytes representing the raw digest ("hash") value.
   */
  digest: function(input, digest_size) {
    input = ecoh512.internal.inputToBytes(input);

    var ctx = ecoh512.init(digest_size);
    ecoh512.update(ctx, input);
    var bytes = ecoh512.final(ctx);
    ecoh512.cleanup(ctx);

    return bytes;
  },

  /**
   * Calculates the ecoh512 message digest ("hash") for the input bytes or string
   * @param {Object} input the input value to hash - either Uint8Array or String
   * @param {Number} digest_size the number of bits for the digest size. 512 is the default
   * @return {String} a hexadecimal representation of the digest ("hash") bytes.
   */
  digestHex: function(input, digest_size) {
    var bytes = ecoh512.digest(input, digest_size);
    return ecoh512.internal.toHex(bytes);
  }
};

createECOH512Module().then(async module => {
  // Memory allocations helpers
  ecoh512.internal.create_buffer  = module.cwrap('malloc', 'number', ['number']);
  ecoh512.internal.destroy_buffer = module.cwrap('free',   '',       ['number']);

  ecoh512.internal.init    = module.cwrap('ecoh512_init',    'number', []);
  ecoh512.internal.update  = module.cwrap('ecoh512_update',  '',       ['number','number','number']);
  ecoh512.internal.final   = module.cwrap('ecoh512_final',   '',       ['number','number']);
  ecoh512.internal.cleanup = module.cwrap('ecoh512_cleanup', '',       ['number']);
  ecoh512.internal.module  = module;
});

