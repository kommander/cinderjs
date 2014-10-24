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

module.exports = Trig;
