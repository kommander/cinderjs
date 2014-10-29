//
// Material
// Wrapping plain gl material calls with ids into a convinience object

var Color = require('color').Color;

var MaterialWrapper = function MaterialWrapper(){
  this._handle = material.create();
  
  // Defaults
  this._ambient = new Color();
  this._diffuse = new Color();
  this._specular = new Color();
  this._emission = new Color();
  this._shininess = 0;
}

exports.Material = MaterialWrapper;

MaterialWrapper.prototype.destroy = function(){
  this._handle = null;
  delete this._handle;
}


MaterialWrapper.prototype.apply = function(){
  material.apply( this._handle.id );
}

MaterialWrapper.prototype.setAmbient = function( r, g, b, a ){
  this._ambient.set( r, g, b, a );
  material.setAmbient(
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
  material.setDiffuse(
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
  material.setSpecular(
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
  material.setEmission(
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
  material.setShininess(
    this._handle.id, s
  );
}
MaterialWrapper.prototype.shininess = MaterialWrapper.prototype.setShininess;


