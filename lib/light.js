//
// Light
var Color = require('color').Color;
var self = this;

var LightWrapper = function LightWrapper(type){
  this._handle = {};
  self.light.create(this._handle, type || LightWrapper.DIRECTIONAL);
  
  // Defaults
  this._ambient = new Color(.9, .9, .9, 1);
  this._diffuse = new Color(.9, .9, .9, 1);
  this._specular = new Color(.9, .9, .9, 1);
}

// Convinience flag for type checking
LightWrapper.isLight = true;

// Types
LightWrapper.POINT = self.light.LIGHT_POINT;
LightWrapper.DIRECTIONAL = self.light.LIGHT_DIRECTIONAL;
LightWrapper.SPOTLIGHT = self.light.LIGHT_SPOTLIGHT;

exports.Light = LightWrapper;

LightWrapper.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

LightWrapper.prototype.destroy = function(){
  self.light.destroy(this._handle.id);
  this._handle.id = null;
}

LightWrapper.prototype.enable = function(){
  self.light.enable( this._handle.id );
}

LightWrapper.prototype.disable = function(){
  self.light.disable( this._handle.id );
}

LightWrapper.prototype.setAmbient = function( r, g, b, a ){
  this._ambient.set( r, g, b, a );
  self.light.setAmbient(
    this._handle.id,
    this._ambient.r,
    this._ambient.g,
    this._ambient.b,
    this._ambient.a
  );
}
LightWrapper.prototype.ambient = LightWrapper.prototype.setAmbient;

LightWrapper.prototype.setDiffuse = function( r, g, b, a ){
  this._diffuse.set( r, g, b, a );
  self.light.setDiffuse(
    this._handle.id,
    this._diffuse.r,
    this._diffuse.g,
    this._diffuse.b,
    this._diffuse.a
  );
}
LightWrapper.prototype.diffuse = LightWrapper.prototype.setDiffuse;

LightWrapper.prototype.setSpecular = function( r, g, b, a ){
  this._specular.set( r, g, b, a );
  self.light.setSpecular(
    this._handle.id,
    this._specular.r,
    this._specular.g,
    this._specular.b,
    this._specular.a
  );
}
LightWrapper.prototype.specular = LightWrapper.prototype.setSpecular;

LightWrapper.prototype.setPosition = function( x, y, z ){
  self.light.setPosition(
    this._handle.id, x, y, z
  );
}
LightWrapper.prototype.position = LightWrapper.prototype.setPosition;

LightWrapper.prototype.lookAt = function( ex, ey, ez, tx, ty, tz ){
  self.light.lookAt(
    this._handle.id, ex, ey, ez, tx, ty, tz
  );
}
