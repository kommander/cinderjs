//
// Vao
var self = this;

var Vao = function Vao() {
  if(!(this instanceof Vao)){
    return new Vao();
  }
  
  this._handle = {};
  self.vao.create(this._handle);
  
};

// Convinience flag for type checking
Vao.prototype.__defineGetter__('isVao', function(){ return true; });
Vao.prototype.__defineGetter__('isBuffer', function(){ return true; });
Vao.prototype.__defineGetter__('bufferType', function(){ return 1; });

module.exports = Vao;

Vao.prototype.__defineGetter__('id', function(){
  return this._handle.id;
});

Vao.prototype.destroy = function(){
  self.vao.destroy(this._handle.id);
  this._handle = null;
};

Vao.prototype.bind = function(){
  self.vao.bind(this._handle.id);
};

Vao.prototype.unbind = function(){
  self.vao.unbind(this._handle.id);
};
