var timers = require('timers');

log("running test...");

// Run a lot of timeouts at the same time
for(var i = 0; i < 100; i++){
  timers.setTimeout(function(now){
    log("Timed out in " + (Date.now() - now) + "ms");
  }, 2000, Date.now());
}

function doTimeout(){
  timers.setTimeout(function(now){
    log("Timed out in " + (Date.now() - now) + "ms");
    doTimeout();
  }, 100, Date.now());
}
doTimeout();