var timers = require('timers');

timers.setTimeout(function(now){
  log("Timed out in " + (Date.now() - now) + "ms");
}, 100, Date.now());