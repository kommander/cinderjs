//
// Batch
var self = this;

var Batch = function Batch(geomMesh, glslProg) {
  this._handle = {};

  if(!glslProg.isShader){
    throw new TypeError('Need GlslProg shader to create a batch.');
  }

  self.batch.create(this._handle, geomMesh, glslProg.id);
}

// Convinience flag for type checking
Batch.prototype.__defineGetter__('isBatch', function(){ return true; });

module.exports = Batch;

// TODO: Use factory methods Shader.fromFile(...) and Shader.fromString(...)

Batch.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

Batch.prototype.destroy = function(){
  self.batch.destroy(this._handle.id);
  this._handle = null;
}

Batch.prototype.draw = function(){
  self.batch.draw(this._handle.id);
}

//
// VertBatch

var VertBatch = Batch.VertBatch = function VertBatch() {
  this._handle = {};
  self.batch.createVert(this._handle);
}

// Convinience flag for type checking
VertBatch.prototype.__defineGetter__('isVertBatch', function(){ return true; });


// TODO: Use factory methods Shader.fromFile(...) and Shader.fromString(...)

VertBatch.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

VertBatch.prototype.destroy = function(){
  self.batch.destroyVert(this._handle.id);
  this._handle = null;
}

VertBatch.prototype.draw = function(){
  self.batch.drawVert(this._handle.id);
}

VertBatch.prototype.color = function( r, g, b ){
  if(r.isColor){
    self.batch.colorVert(this._handle.id, r.r, r.g, r.y);
  } else {
    self.batch.colorVert(this._handle.id, r, g, b);
  }
  return this;
}

VertBatch.prototype.vertex = function( x, y, z ){
  if(z){
    self.batch.vertexVert(this._handle.id, x, y, z);
  } else {
    self.batch.vertexVert(this._handle.id, x, y);
  }
  return this;
}
