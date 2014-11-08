//
// Camera
var Ray = require('ray');
var self = this;

var Camera = function Camera(){
  this._handle = {};
  if(arguments.length == 0){
    self.camera.create(this._handle);
  } else if(arguments.length == 3){
    self.camera.create(this._handle, arguments[0], arguments[1], arguments[2]);
  }
  
  // Defaults
  // TODO: set usable defaults
  this._eyePoint = { x: 0.0, y: 0.0, z: 0.0 };
  this._lookAt = { x: 0.0, y: 0.0, z: 0.0 };
  this._direction = { x: 0.0, y: 0.0, z: 0.0 };
  this._orientation = { x: 0.0, y: 0.0, z: 0.0 };

}

// Convinience flag for type checking
Camera.prototype.__defineGetter__('isCamera', function(){ return true; });

module.exports = Camera;

Camera.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

Camera.prototype.destroy = function(){
  self.camera.destroy(this._handle.id);
  this._handle = null;
}

Camera.prototype.setEyePoint = function( x, y, z ){
  self.camera.setEyePoint(
    this._handle.id, x, y, z
  );
}

Camera.prototype.lookAt = function( x1, y1, z1, x2, y2, z2 ){
  self.camera.lookAt(
    this._handle.id, x1, y1, z1, x2, y2, z2
  );
}

Camera.prototype.setViewDirection = function( x, y, z ){
  self.camera.setViewDirection(
    this._handle.id, x, y, z
  );
}

Camera.prototype.setOrientation = function( x, y, z ){
  self.camera.setOrientation(
    this._handle.id, x, y, z
  );
}

Camera.prototype.generateRay = function( u, v, aRatio ){
  var rayHandle = {};
  self.camera.generateRay( this._handle.id, u, v, aRatio, rayHandle );
  var ray = new Ray( rayHandle );
  return ray;
}

// (float verticalFovDegrees, float aspectRatio, float nearPlane, float farPlane)
Camera.prototype.setPerspective = function( verticalFovDegrees, aspectRatio, nearPlane, farPlane ){
  self.camera.setPerspective( this._handle.id,
    verticalFovDegrees,
    aspectRatio,
    nearPlane,
    farPlane
  );
}

Camera.prototype.setCenterOfInterestPoint = function( x, y, z ){
  self.camera.setCenterOfInterestPoint(
    this._handle.id, x, y, z
  );
}

