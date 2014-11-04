//
// Material
// Wrapping plain gl material calls with ids into a convinience object

var Color = require('color').Color;
var self = this;

var MaterialWrapper = function MaterialWrapper(){
  this._handle = {};
  self.material.create(this._handle);
  
  // Defaults
  this._ambient = new Color();
  this._diffuse = new Color();
  this._specular = new Color();
  this._emission = new Color();
  this._shininess = 0;
}

module.exports = MaterialWrapper;

MaterialWrapper.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

MaterialWrapper.prototype.destroy = function(){
  self.material.destroy(this._handle.id);
  this._handle = null;
  delete this._handle;
}


MaterialWrapper.prototype.apply = function(){
  self.material.apply( this._handle.id );
}

MaterialWrapper.prototype.setAmbient = function( r, g, b, a ){
  this._ambient.set( r, g, b, a );
  self.material.setAmbient(
    this._handle.id,
    this._ambient.r,
    this._ambient.g,
    this._ambient.b,
    this._ambient.a
  );
}
MaterialWrapper.prototype.ambient = MaterialWrapper.prototype.setAmbient;

MaterialWrapper.prototype.setDiffuse = function( r, g, b, a ){
  this._diffuse.set( r, g, b, a );
  self.material.setDiffuse(
    this._handle.id,
    this._diffuse.r,
    this._diffuse.g,
    this._diffuse.b,
    this._diffuse.a
  );
}
MaterialWrapper.prototype.diffuse = MaterialWrapper.prototype.setDiffuse;

MaterialWrapper.prototype.setSpecular = function( r, g, b, a ){
  this._specular.set( r, g, b, a );
  self.material.setSpecular(
    this._handle.id,
    this._specular.r,
    this._specular.g,
    this._specular.b,
    this._specular.a
  );
}
MaterialWrapper.prototype.specular = MaterialWrapper.prototype.setSpecular;

MaterialWrapper.prototype.setEmission = function( r, g, b, a ){
  this._emission.set( r, g, b, a );
  self.material.setEmission(
    this._handle.id,
    this._emission.r,
    this._emission.g,
    this._emission.b,
    this._emission.a
  );
}
MaterialWrapper.prototype.emission = MaterialWrapper.prototype.setEmission;

MaterialWrapper.prototype.setShininess = function( s ){
  this._shininess = s;
  self.material.setShininess(
    this._handle.id, s
  );
}
MaterialWrapper.prototype.shininess = MaterialWrapper.prototype.setShininess;


