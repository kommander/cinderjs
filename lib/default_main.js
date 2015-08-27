//
// Default
// Is loaded when no module was specified in command line arguments

// here we can listen for a file drop or create menue to load js files
// TODO: Make reloading main modules possible
// TODO: unload previous main module
// TODO: Run dropped file in separate context, replacable when another file is dropped
// TODO: Make dropping a folder work

var Module = require('module').Module;
var gl = require('gl');

var currentModule = null;

// TODO: Main modules should request access to certain native features, like files or networking.
//       Popup a dialog asking for permissions and if granted run the module. 
//       (Browser behaviour -> load .cjs scripts from Urls)      
//       (When a module from another domain requests access, ask again etc.)
//       In paranoid mode, ask for every single file/network access by a script

app.on('filedrop', function( evt ){
  if(evt.files.length == 1 && evt.files[0].match(/\.js/)){
    // FIXME: Previous module is not replaced correctly (dirty)
    Module._load(evt.files[0], null, false);
  }
});

app.draw(function(){
  gl.clear( 0.1, 0.1, 0.1 );
});

app.on('keydown', function( evt ){
  console.log('keydown: ' + evt.charCode);
});