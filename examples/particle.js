// Some Trigonometry helper methods

var Trig = {
  PI180: Math.PI / 180,
  
  /**
   * Convert a degree to a radian 
   */
  radian: function(d) {
    return d * PI180;
  },

  /**
   * Convert a radian to a degree 
   */
  degree: function(r) {
    return r / PI180;
  },

  /**
   * Find the angle of trajectory from two given points 
   */
  getAngle: function(a, b) {
    return Math.atan2(b.y - a.y, b.x - a.x);
  },

  /**
   * Get the distance from two given points 
   */
  getDistance: function(a, b) {
    return Math.sqrt(this.getSquaredDistance(a, b));
  },

  /**
   * Get the distance without applying square root for comparison
   */
  getSquaredDistance: function(a, b) {
    return Math.pow(b.y - a.y, 2) + Math.pow(b.x - a.x, 2);
  }
}

// Vector2D
var Vec2 = function(x, y, angle) {
  if(!(this instanceof Vec2)){
    return new Vec2( x, y, angle );
  }

  this.angle = angle;
  this.x = x;
  this.y = y;
};
  
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

function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

// Screen size holder
var ctxSize = {
  x: 640,
  y: 480
};

var moveParticles = true;
var drawMethod = 1;
var lastParticle;
var mouse = new Vec2();
var mouseActive = true;

var particles = [];
function generateParticles( amt ){
  for( var i = amt; i > 0; i-- ) {
    particles.push( newParticle() );
  }
  lastParticle = particles[0];
}

function newParticle(){
  var particle = new Vec2( getRandomInt(0, ctxSize.x), getRandomInt(0, ctxSize.y), getRandomInt(0, 360) );
  particle.radius = getRandomInt(2, 5);
  particle.vel = getRandomInt(1, 4);
  particle.rgb = [1,1,1];
  return particle;
}

removeParticles = function( amt ) {
  particles.splice(0, amt);
};

var particle;
var i;
var colorBuf = 0;
var distance;

var loop = function(){
  for( i = particles.length - 1; i >= 0; i-- ) {
    particle = particles[i];
    if(particle.x < 0 || particle.x > ctxSize.x || particle.y < 0 || particle.y > ctxSize.y){
      particle.reverse();
    }
    if(moveParticles) {
      distance = particle.distance(mouse)
      if(mouseActive && distance < 80) {
        particle.rgb[0] = 1 - 2 / distance;
        particle.rgb[1] = 1 - 1 / distance;
        particle.rgb[2] = 1 - .5 / distance;
        particle.angleTo(mouse);
        if(mouse.down) {
          particle.reverse();
        }
        particle.forward(particle.vel * 2 );
      } else {
        particle.forward(particle.vel);  
      }
    }
    //gl.color(particle.rgb[0], particle.rgb[1], particle.rgb[2]);
    
    if(drawMethod == 0) {
     gl.drawSolidCircle( particle.x, particle.y, particle.radius );
    } else if(drawMethod == 1) {
      gl.drawLine(lastParticle.x, lastParticle.y, particle.x, particle.y);
      lastParticle = particle;
    }
  }
}

__draw__(function(mx, my){
  // Update mouse position
  mouse.x = mx;
  mouse.y = my;
  
  loop();
});

__event__(function( type ){
  // Resize Event
  if(type == 10){
    ctxSize.x = arguments[1];
    ctxSize.y = arguments[2];
  }
  // Key Down
  else if(type == 20){
    var charCode = arguments[1];
    var char = arguments[2];
    log("KeyDown " + charCode + " / " + char);
  }
  // Key Up
  else if(type == 30){
    var charCode = arguments[1];
    var char = arguments[2];
    log("KeyUp " + charCode + " / " + char);
  }
});

// Add some particles
generateParticles(2000);
