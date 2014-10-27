//
// Camera
var Ray = require('ray').Ray;

var CameraWrapper = function CameraWrapper(){
  this.id = camera.create();
  
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

CameraWrapper.prototype.setEyePoint = function( x, y, z ){
  camera.setEyePoint(
    this.id, x, y, z
  );
}

CameraWrapper.prototype.lookAt = function( x, y, z ){
  camera.lookAt(
    this.id, x, y, z
  );
}

CameraWrapper.prototype.setViewDirection = function( x, y, z ){
  camera.setViewDirection(
    this.id, x, y, z
  );
}

CameraWrapper.prototype.setOrientation = function( x, y, z ){
  camera.setOrientation(
    this.id, x, y, z
  );
}

CameraWrapper.prototype.generateRay = function( u, v, aRatio ){
  var rayId = camera.generateRay( this.id, u, v, aRatio );
  var ray = new Ray( rayId );
  return ray;
}

// (float verticalFovDegrees, float aspectRatio, float nearPlane, float farPlane)
CameraWrapper.prototype.setPerspective = function( verticalFovDegrees, aspectRatio, nearPlane, farPlane ){
  camera.setPerspective( this.id,
    verticalFovDegrees,
    aspectRatio,
    nearPlane,
    farPlane
  );
}

CameraWrapper.prototype.setCenterOfInterestPoint = function( x, y, z ){
  camera.setCenterOfInterestPoint(
    this.id, x, y, z
  );
}

