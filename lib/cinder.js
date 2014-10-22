// Main JS application entry point
(function(process) {

  // Create a new global object which will be used by non-native modules
  this.global = {};
 
  var path = process.nativeBinding('path');
 
  function start() {
    toggleAppConsole(true);
    log('Hello Cinder');
 
    // Check process.argv for js file to run
    for( var i = 0; i < process.argv.length; ++i ){
      log("argv[" + i + "]: " + process.argv[i]);
    }
 
    // TODO: get process.argv, load the user module and execute it
    // TODO: look for index.js next to app executable and load it
    // TODO: if no js file to run is given in argv (or index.js next to executable), wait for file drop on app (if is a folder, check for index.js)
 
    process.nextFrame(function(){
      log("Got executed next frame");
    });
    
  }

  start();
  
}); // module wrapper
