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