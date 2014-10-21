//
// Cubes Example

// Screen size holder
var ctxSize = {
  x: 640,
  y: 480
};

// Mouse
var mouse = {
  x: 0,
  y: 0
};

var wireframe = false;

var loop = function(timePassed){
  gl.pushMatrices();
  if(wireframe) gl.enableWireframe();
  gl.translate(mouse.x, mouse.y);
  gl.drawCube(0, 0, 0, 60, 60, 60);
  if(wireframe) gl.disableWireframe();
  gl.popMatrices();
}

__draw__(function(timePassed, mx, my){
  // Update mouse position
  mouse.x = mx;
  mouse.y = my;
  
  loop(timePassed);
});

__event__(function( type ){
  // Resize Event
  if(type == 10){
    //ctxSize.x = arguments[1];
    //ctxSize.y = arguments[2];
  }
  // Key Down
  else if(type == 20){
    var charCode = arguments[1];
    var char = arguments[2];
    
    if(charCode == 99) { // C
      toggleAppConsole();
    } else if(charCode == 27) { // ESC
      quit();
    } else if(charCode == 118) { // V
      toggleV8Stats();
    } else if(charCode == 119) { // W
      wireframe = !wireframe;
    }
  }
});