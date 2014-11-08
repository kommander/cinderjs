//
// Texture
var self = this;

// TODO: Check texture format when implemented
var TextureWrapper = function TextureWrapper(imagePath, textureFormat) {
  if(!imagePath) return;
  
  this._handle = {};

  self.texture.create(this._handle, imagePath, textureFormat);
}

// Convinience flag for type checking
TextureWrapper.prototype.__defineGetter__('isTexture', function(){ return true; });

module.exports = TextureWrapper;

// TODO: Use factory methods Shader.fromFile(...) and Shader.fromString(...)

TextureWrapper.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

TextureWrapper.prototype.destroy = function(){
  self.texture.destroy(this._handle.id);
  this._handle = null;
}

TextureWrapper.prototype.bind = function(){
  self.texture.bind(this._handle.id);
}

