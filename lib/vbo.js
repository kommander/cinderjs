//
// Vbo
var self = this;

/**
 * The native implementation takes the byte length for the buffer from the array type,
 * which is given as data argument. If data is a number, an empty Vbo with that number as size
 * is allocated.
 */
var Vbo = function Vbo( target, data, usage ) {
  if(!(this instanceof Vbo)){
    return new Vbo(target, data, usage);
  }
  
  this._handle = {};
  
  if(arguments.length == 1){
    self.vbo.create(this._handle, target);
  } 
  else {
    self.vbo.create(this._handle, target, data, usage);
  }

  this._data = data;

};

Vbo.prototype.__defineGetter__('size', function(){ return this._data.length; });

// Convinience flag for type checking
Vbo.prototype.__defineGetter__('isVbo', function(){ return true; });
Vbo.prototype.__defineGetter__('isBuffer', function(){ return true; });
Vbo.prototype.__defineGetter__('bufferType', function(){ return 2; });

module.exports = Vbo;

Vbo.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

Vbo.prototype.destroy = function(){
  self.vbo.destroy(this._handle.id);
  this._handle = null;
};

Vbo.prototype.bind = function(){
  self.vbo.bind(this._handle.id);
};

Vbo.prototype.unbind = function(){
  self.vbo.unbind(this._handle.id);
};
