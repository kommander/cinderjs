//
// GL Wraps

var native_scopedShader = this.gl.scopedShader;
this.gl.scopedShader = function( shader ){
  if(!shader.isShader){
    throw new TypeError('Need a shader.');
  }
  native_scopedShader(shader.id);
}

module.exports = this.gl;