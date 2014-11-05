//
// GLM
var self = this;

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
