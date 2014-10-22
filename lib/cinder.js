// Main JS application entry point
// Note: Wrappers need to be in place for the main module
(function(process) {

  // Create a new global object which will be used by non-native modules
  this.global = {};
 
  function start() {
    var path = process.nativeBinding('path');
 
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
 

//  NativeModule.exists = function(id) {
//    // TODO: expose native module names
//    return NativeModule._source.hasOwnProperty(id);
//  } 
 
  // Set wrappers for natives and reuse in module.js
  process.wrap = function(script) {
    return process.moduleWraps[0] + script + process.moduleWraps[1];
  };
 
  process.moduleWraps = [
    '(function (exports, require, module, __filename, __dirname) { ',
    '\n});'
  ];

  start();
  
}); // module wrapper
