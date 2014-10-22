// Main JS application entry point
(function(process) {
  this.global = this;

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
    
  }

  start();
});
