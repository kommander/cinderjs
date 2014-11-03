//
// Camera
var Ray = require('ray').Ray;

var CameraWrapper = function CameraWrapper(){
  this._handle = {};
  camera.create(this._handle);
  
  // Defaults
  // TODO: set usable defaults
  this._eyePoint = { x: 0.0, y: 0.0, z: 0.0 };
  this._lookAt = { x: 0.0, y: 0.0, z: 0.0 };
  this._direction = { x: 0.0, y: 0.0, z: 0.0 };
  this._orientation = { x: 0.0, y: 0.0, z: 0.0 };

}

// Convinience flag for type checking
CameraWrapper.isCamera = true;

exports.Camera = CameraWrapper;

CameraWrapper.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

CameraWrapper.prototype.destroy = function(){
  this._handle = null;
}

CameraWrapper.prototype.setEyePoint = function( x, y, z ){
  camera.setEyePoint(
    this._handle.id, x, y, z
  );
}

CameraWrapper.prototype.lookAt = function( x, y, z ){
  camera.lookAt(
    this._handle.id, x, y, z
  );
}

CameraWrapper.prototype.setViewDirection = function( x, y, z ){
  camera.setViewDirection(
    this._handle.id, x, y, z
  );
}

CameraWrapper.prototype.setOrientation = function( x, y, z ){
  camera.setOrientation(
    this._handle.id, x, y, z
  );
}

CameraWrapper.prototype.generateRay = function( u, v, aRatio ){
  var rayHandle = {};
  camera.generateRay( this._handle.id, u, v, aRatio, rayHandle );
  var ray = new Ray( rayHandle );
  return ray;
}

// (float verticalFovDegrees, float aspectRatio, float nearPlane, float farPlane)
CameraWrapper.prototype.setPerspective = function( verticalFovDegrees, aspectRatio, nearPlane, farPlane ){
  camera.setPerspective( this._handle.id,
    verticalFovDegrees,
    aspectRatio,
    nearPlane,
    farPlane
  );
}

CameraWrapper.prototype.setCenterOfInterestPoint = function( x, y, z ){
  camera.setCenterOfInterestPoint(
    this._handle.id, x, y, z
  );
}

