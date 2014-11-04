//
// Main JS application entry point
//
// Note: Wrappers need to be in place for the main module -> (function(process) { ... });
// Note: To check if a native module exists, a second bool argument can be given to process.nativeBinding
//       (or require() within modules) which will not load the module, but check for its existence if true.
//       It will return a bool value which can be absolutely checked with an === operator.
(function(process) {

  var global = this;
  global.global = global;
 
  function start() {
 
    var EventEmitter = process.nativeBinding('events').EventEmitter;
 
    // Setup process
    process.__proto__ = Object.create(EventEmitter.prototype, {
      constructor: {
        value: process.constructor
      }
    });
    EventEmitter.call(process);
 
    // Setup app namespace
    var app = process.nativeBinding('app');
    app.__proto__ = Object.create(EventEmitter.prototype, {
      constructor: {
        value: app.constructor
      }
    });
    EventEmitter.call(app);
 
    global.app = app;
 
    app.draw = __draw__;
    delete __draw__;
    
    var handleRawEvent = function( type ){
      // Resize Event
      if(type == 10){
        app.emit('resize', arguments[1], arguments[2]);
      }
      // Key Down
      else if(type == 20){
        // TODO: Make real key event with goodies
        app.emit('keydown', {
          charCode: arguments[1],
          char: arguments[2]
        });
      }
      // Key Up
      else if(type == 30){
        // TODO: Make real key event with goodies
        app.emit('keyup', {
          charCode: arguments[1],
          char: arguments[2]
        });
      }
      
      // File Drop
      else if(type == 100){
        app.emit('filedrop', {
          files: arguments[1]
        });
      }

    };

    // Setup event handler and remove raw from global
    __event__(handleRawEvent);
    delete __event__;

    // Have global console
    global.console = process.nativeBinding('console');
 
    // Do not try to load any native modules before this point,
    // as the wrappers are set at the end of this file (leave em)
    var path = process.nativeBinding('path');
    var util = process.nativeBinding('util');
    var Module = process.nativeBinding('module');
 
    // TODO: Inititalize console.js, exposing the global console object
    //       (with default keyboard shortcut to bring up the gui console)
 
    // TODO: Setup global app namespace, wrap __draw__ and __event__ and delete it afterwards so it may not be overridden.
    //       Set app.draw() to be a getter only so the main render loop may not be f'ed with.
 
    // TODO: Setup event emitter
 
    // Setup timers
    var timers = process.nativeBinding('timers');
    for(k in timers){
      global[k] = timers[k];
    }

    // TODO: Check if can be run in next frame, so startup here doesn't need a GL context
    var result = Module.runMain();
 
    // If there was no main module to load, load default main from natives
    // TODO: if no js file to run is given in argv (or index.js next to executable), wait for file drop on app (if is a folder, check for index.js)
    if(result === false){
      Module._load('default_main', null, true);
    }

  }
 
  // Set wrappers for natives and reuse in module.js
  process.wrap = function(script) {
    return process.moduleWraps[0] + script + process.moduleWraps[1];
  };
 
  process.moduleWraps = [
    '(function (exports, require, module, __filename, __dirname) { ',
    '\n});'
  ];

  //
  // Bootstrap helpers
  getRandomInt = function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
  }

  start();
  
}); // module wrapper
