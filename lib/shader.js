//
// Shader
var self = this;

var ShaderWrapper = function ShaderWrapper(vert, frag) {
  this._handle = {};
  self.shader.create(this._handle, vert, frag);
}

// Convinience flag for type checking
ShaderWrapper.prototype.__defineGetter__('isShader', function(){ return true; });

module.exports = ShaderWrapper;

// TODO: Use factory methods Shader.fromFile(...) and Shader.fromString(...)

ShaderWrapper.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

ShaderWrapper.prototype.destroy = function(){
  self.shader.destroy(this._handle.id);
  this._handle = null;
}

