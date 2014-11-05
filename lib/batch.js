//
// Batch
var self = this;

var BatchWrapper = function BatchWrapper(geomMesh, glslProg) {
  this._handle = {};

  if(!glslProg.isShader){
  	throw new TypeError('Need GlslProg shader to create a batch.');
  }

  self.batch.create(this._handle, geomMesh, glslProg.id);
}

// Convinience flag for type checking
BatchWrapper.prototype.__defineGetter__('isBatch', function(){ return true; });

module.exports = BatchWrapper;

// TODO: Use factory methods Shader.fromFile(...) and Shader.fromString(...)

BatchWrapper.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

BatchWrapper.prototype.destroy = function(){
  self.batch.destroy(this._handle.id);
  this._handle = null;
}

BatchWrapper.prototype.draw = function(){
  self.batch.draw(this._handle.id);
}

