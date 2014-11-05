//
// Color

var Color = function Color(r, g, b, a){
  this.set( r, g, b, a );
}

// Convinience flag for type checking
Color.prototype.__defineGetter__('isColor', function(){ return true; });


exports.Color = Color;

Color.prototype.set = function( r, g, b, a ){
  if(r && r.isColor) {
    this.r = r.r;
    this.g = r.g;
    this.b = r.b;
    this.a = r.a;
  } else {
    this.r = typeof r === 'number' ? r : 1;
    this.g = typeof g === 'number' ? g : 1;
    this.b = typeof b === 'number' ? b : 1;
    this.a = typeof a === 'number' ? a : 1;
  }
}

