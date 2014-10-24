//
// Default
// Is loaded when no module was specified in command line arguments

// here we can listen for a file drop or create menue to load js files
// TODO: Make reloading main modules possible
// TODO: unload previous main module
// TODO: load in separate context with clean global (to not inherit mess from previous module)
// TODO: Make dropping a folder work

var Module = require('module').Module;

var currentModule = null;

app.on('filedrop', function( evt ){
  if(evt.files.length == 1 && evt.files[0].match(/\.js/)){
    Module._load(evt.files[0], null, false);
  }
});