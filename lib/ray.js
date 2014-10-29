//
// Ray

var RayWrapper = function RayWrapper( handle ){
  if(!handle){
    this._handle = ray.create();
  } else {
    this._handle = handle;
  }
  
  this.id = this._handle.id;
  
  // Defaults
  this._origin = { x: 0.0, y: 0.0, z: 0.0 };
  this._direction = { x: 0.0, y: 0.0, z: 0.0 };
}

// Convinience flag for type checking
RayWrapper.isRay = true;

exports.Ray = RayWrapper;

RayWrapper.prototype.destroy = function(){
  this._handle = null;
}

RayWrapper.prototype.setOrigin = function( x, y, z ){
  ray.setOrigin(
    this._handle.id, x, y, z
  );
}
// TODO: getter and setter for
//RayWrapper.prototype.origin;

RayWrapper.prototype.getOrigin = function(){
  // Use js land buffer object, no need atm. to get the origin from C Object
  return this._origin;
}

RayWrapper.prototype.setDirection = function( x, y, z ){
  ray.setDirection(
    this._handle.id, x, y, z
  );
}
// TODO: getter and setter for
//RayWrapper.prototype.direction;

RayWrapper.prototype.getDirection = function(){
  // Use js land buffer object, no need atm. to get the direction from C Object
  return this._direction;
}

RayWrapper.prototype.calcPosition = function( t ){
  var pos = ray.calcPosition( this._handle.id, t );
  return {
    x: pos[0],
    y: pos[1],
    z: pos[2]
  }
} 

RayWrapper.prototype.calcTriangleIntersection = function( v1, v2, v3 ){
  return ray.calcTriangleIntersection( 
    this._handle.id,
    v1.x, v1.y, v1.z,
    v2.x, v2.y, v2.z,
    v3.x, v3.y, v3.z
  );
} 

