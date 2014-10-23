var timers = require('timers');

for(var i = 0; i < 100; i++){
  timers.setTimeout(function(now){
    log("Timed out in " + (Date.now() - now) + "ms");
  }, 2000, Date.now());
}