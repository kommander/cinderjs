//
// Timers

var util = require('util');

// Timeout values > TIMEOUT_MAX are set to 1.
var TIMEOUT_MAX = 2147483647; // 2^31-1

exports.setTimeout = function(callback, after) {
  var timer = createTimer.apply(this, arguments);
  timer.start();
  return timer;
};

exports.setInterval = function(callback, after) {
  var timer = createTimer.apply(this, arguments);
  timer.repeat = true;
  timer.start();
  return timer;
};

function clear(timer){
  if(util.isObject(timer) && util.isFunction(timer.clear)){
    timer.clear();
  }
}

exports.clearTimeout = function(timer){
  clear(timer);
}

exports.clearInterval = function(timer){
  clear(timer);
}

var createTimer = function(callback, after){
  var timer;

  after *= 1; // coalesce to number or NaN

  if (!(after >= 1 && after <= TIMEOUT_MAX)) {
    after = 1; // schedule on next tick, follows browser behaviour
  }

  timer = new Timeout(after);

  if (arguments.length <= 2) {
    timer._onTimeout = callback;
  } else {
    /*
     * Sometimes setTimeout is called with arguments, EG
     *
     *   setTimeout(callback, 2000, "hello", "world")
     *
     * If that's the case we need to call the callback with
     * those args. The overhead of an extra closure is not
     * desired in the normal case.
     */
    var args = Array.prototype.slice.call(arguments, 2);
    timer._onTimeout = function() {
      callback.apply(timer, args);
    }
  }
  
  return timer;
}

var Timeout = function(after, repeat){
  this.id = Timeout._ids++;
  this.after = after;
  this.repeat = repeat;
  this._onTimeout = null;
}

Timeout._ids = 0;

Timeout.prototype.start = function(){
  this._ref = setTimer(this.id, this._onTimeout, this.after, this.repeat);
}

Timeout.prototype.clear = function(){
  // Calling setTimer with id only should remove the timer
  this._ref = setTimer(this.id);
}