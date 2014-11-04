//
// Camera
var Ray = require('ray').Ray;
var self = this;

var CameraWrapper = function CameraWrapper(){
  this._handle = {};
  self.camera.create(this._handle);
  
  // Defaults
  // TODO: set usable defaults
  this._eyePoint = { x: 0.0, y: 0.0, z: 0.0 };
  this._lookAt = { x: 0.0, y: 0.0, z: 0.0 };
  this._direction = { x: 0.0, y: 0.0, z: 0.0 };
  this._orientation = { x: 0.0, y: 0.0, z: 0.0 };

}

// Convinience flag for type checking
CameraWrapper.isCamera = true;

module.exports = CameraWrapper;

CameraWrapper.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

CameraWrapper.prototype.destroy = function(){
  self.camera.destroy(this._handle.id);
  this._handle = null;
}

CameraWrapper.prototype.setEyePoint = function( x, y, z ){
  self.camera.setEyePoint(
    this._handle.id, x, y, z
  );
}

CameraWrapper.prototype.lookAt = function( x, y, z ){
  self.camera.lookAt(
    this._handle.id, x, y, z
  );
}

CameraWrapper.prototype.setViewDirection = function( x, y, z ){
  self.camera.setViewDirection(
    this._handle.id, x, y, z
  );
}

CameraWrapper.prototype.setOrientation = function( x, y, z ){
  self.camera.setOrientation(
    this._handle.id, x, y, z
  );
}

CameraWrapper.prototype.generateRay = function( u, v, aRatio ){
  var rayHandle = {};
  self.camera.generateRay( this._handle.id, u, v, aRatio, rayHandle );
  var ray = new Ray( rayHandle );
  return ray;
}

// (float verticalFovDegrees, float aspectRatio, float nearPlane, float farPlane)
CameraWrapper.prototype.setPerspective = function( verticalFovDegrees, aspectRatio, nearPlane, farPlane ){
  self.camera.setPerspective( this._handle.id,
    verticalFovDegrees,
    aspectRatio,
    nearPlane,
    farPlane
  );
}

CameraWrapper.prototype.setCenterOfInterestPoint = function( x, y, z ){
  self.camera.setCenterOfInterestPoint(
    this._handle.id, x, y, z
  );
}

