var Trig = require('./trig.js');

// Vector2D
var Vec2 = function(x, y, angle) {
  if(!(this instanceof Vec2)){
    return new Vec2( x, y, angle );
  }

  this.angle = angle;
  this.x = x;
  this.y = y;
};

module.exports = Vec2;
  
/** 
 * Move based on angle of trajectory and magnitude of distance
 * a - the angle to move in
 * m - how far to move 
 */
Vec2.prototype.move = function(a, m) {
  this.x += Math.cos(a) * m;
  this.y += Math.sin(a) * m;
};

/**
 * Move left based on the direction angle
 * m - how far to move 
 */
Vec2.prototype.left = function(m) {
  this.move(this.angle - 1.5707963267948966192313216916398, m);
};

/**
 * Move right based on the direction angle
 * m - how far to move 
 */
Vec2.prototype.right = function(m) {
  this.move(this.angle + 1.5707963267948966192313216916398, m);
};

/**
 * Move forward based on the direction angle
 * m - how far to move 
 */
Vec2.prototype.forward = function(m) {
  this.move(this.angle, m);
};

/**
 * Move backward based on the direction angle
 * m - how far to move 
 */
Vec2.prototype.backward = function(m) {
  this.move(this.angle - Math.PI, m);
};

/**
 * Set angle to face the given point
 * p - the geometric point 
 */
Vec2.prototype.angleTo = function(p) {
  return this.angle = Trig.getAngle(this, p);
};

/**
 * Other direction
 */
Vec2.prototype.reverse = function() {
  this.angle += Math.PI;
};

/**
 * Get the distance from this instance to a given point
 * p - the geometric point 
 */
Vec2.prototype.distance = function(p) {
  return Trig.getDistance(this, p);
};

/**
 * Get the distance without applying square root for comparison
 */
Vec2.prototype.comparableDistance = function(p) {
  return Trig.getSquaredDistance(this, p);
};

/**
 * Get the dot product of this vector to another vector
 */
Vec2.prototype.dotProduct = function(other) {
  return this.x * other.x + this.y * other.y;
};

/**
 * Returns a new vector with the same attributes as this
 */
Vec2.prototype.copy = function(){
  return new Vec2(this.x, this.y, this.angle);
};
