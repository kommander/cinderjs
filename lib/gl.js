//
// GL Wraps
module.exports = this.gl;

var native_setModelMatrix = this.gl.setModelMatrix;
module.exports.setModelMatrix = function( mat ){
  if(!mat.isMat4){
    throw new TypeError('Need a Mat4 to set model matrix');
  }
  native_setModelMatrix( mat.id );
}

var native_multModelMatrix = this.gl.multModelMatrix;
module.exports.multModelMatrix = function( mat ){
  if(!mat.isMat4){
    throw new TypeError('Need a Mat4 to mult model matrix');
  }
  native_multModelMatrix( mat.id );
}

// TODO: Take Rect object optionally (when implemented)
var native_drawTexture = this.gl.drawTexture;
module.exports.drawTexture = function( texture, rx, ry, rw, rh ){
  if(!texture.isTexture){
    throw new TypeError('Need a Texture');
  }
  native_drawTexture( texture.id, rx, ry, rw, rh );
}

var native_bindBufferBase = this.gl.bindBufferBase;
module.exports.bindBufferBase = function( target, index, buffer ){
  if(!buffer.isBuffer){
    throw new TypeError('Need a buffer to bind');
  }
  native_bindBufferBase( target, index, buffer.id, buffer.bufferType );
}