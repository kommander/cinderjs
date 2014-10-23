//
// Main JS application entry point
//
// Note: Wrappers need to be in place for the main module
// Note: To check if a native module exists, a second bool argument can be given to process.nativeBinding
//       (or require() within modules) which will not load the module, but check for its existence if true.
//       It will return a bool value which can be absolutely checked with an === operator.
(function(process) {

  // TODO: Create a new global object which will be used by non-native modules
  this.global = {};
 
  function start() {
    
    // Do not try to load any native modules before this point,
    // as the wrappers are set at the end of this file (leave em)
    var path = process.nativeBinding('path');
    var util = process.nativeBinding('util');
    var Module = process.nativeBinding('module').Module;
 
    // TODO: Inititalize console.js, exposing the global console object
    //       (with default keyboard shortcut to bring up the gui console)
 
    // TODO: Setup global app namespace, wrap __draw__ and __event__ and delete it afterwards so it may not be overridden.
    //       Set app.draw() to be a getter only so the main render loop may not be f'ed with.
 
    // TODO: Setup event emitter
 
    // Setup timers
 
    // TODO: Check if can be run in next frame, so startup here doesn't need a GL context
    // TODO: if no js file to run is given in argv (or index.js next to executable), wait for file drop on app (if is a folder, check for index.js)
    Module.runMain();
 
//    process.nextFrame(function(){
//      log("Got executed next frame");
//    });
 
  }
 
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
