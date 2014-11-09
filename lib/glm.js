//
// GLM
var self = this;

//
// Mat4
//
var Mat4 = function Mat4( handleOrOther ) {
  
  if(!handleOrOther){
    this._handle = {};
    self.glm.createMat4(this._handle);
  } else if(!handleOrOther.isMat4 && handleOrOther.id) {
    this._handle = handleOrOther;
  }

}

// Convinience flag for type checking
Mat4.prototype.__defineGetter__('isMat4', function(){ return true; });

exports.Mat4 = Mat4;

// TODO: Use factory methods Shader.fromFile(...) and Shader.fromString(...)

Mat4.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

Mat4.prototype.destroy = function(){
  self.glm.destroyMat4(this._handle.id);
  this._handle = null;
}

Mat4.prototype.mult = function( otherMat4 ){
  if(!otherMat4.isMat4){
    throw new TypeError('Need a Mat4 to multiply with.');
  }
  self.glm.multMat4(this._handle.id, otherMat4.id);
}


exports.rotate = function( angle, x, y, z ){
  var handle = {};
  self.glm.rotate( handle, angle, x, y, z );
  return new Mat4( handle );
}

//
// Vec3
//
var Vec3 = function Vec3( x, y, z ) {
  if(!(this instanceof Vec3)) {
    return new Vec3( x, y, z );
  }

  this._data = { x: 0, y: 0, z: 0 };

  if(!x){
    this._handle = {};
    self.glm.createVec3(this._handle);
  } else if(!x.isVec3 && x.id) {
    this._handle = x;
  } else {
    this._handle = {};
    self.glm.createVec3( this._handle, x, y, z );
    this._data.x = x;
    this._data.y = y;
    this._data.z = z;
  }

}

Vec3.prototype.__defineGetter__('x', function(){ return this._data.x; });
Vec3.prototype.__defineGetter__('y', function(){ return this._data.y; });
Vec3.prototype.__defineGetter__('z', function(){ return this._data.z; });

// Convinience flag for type checking
Vec3.prototype.__defineGetter__('isVec3', function(){ return true; });

exports.Vec3 = Vec3;

// TODO: Use factory methods Shader.fromFile(...) and Shader.fromString(...)

Vec3.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

Vec3.prototype.destroy = function(){
  self.glm.destroyVec3(this._handle.id);
  this._handle = null;
}

Vec3.prototype.set = function( x, y, z ){
  self.glm.setVec3(this._handle.id, x, y, z);
  this._data.x = x;
  this._data.y = y;
  this._data.z = z;
}

Vec3.prototype.add = function( x, y, z ){
  if(x.isVec3){
    var result = self.glm.addVec3(this._handle.id, x.id);  
  } else {
    var result = self.glm.addVec3(this._handle.id, x, y, z);
  }
  
  if(result){
    this._data.x = result[0];
    this._data.y = result[1];
    this._data.z = result[2];
  }
}
