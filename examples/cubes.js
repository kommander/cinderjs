//
// Cubes Example
var Material = require('material').Material;
var Light = require('light').Light;

// Screen size holder
var ctxSize = { x: 640, y: 480 };

// Mouse
var mouse = { x: 0, y: 0 };

// Rotation
var rotation = { x: 0, y: 0, z: 0 };

var currentLight = 0;

var drawMethod = 0;
var wireframe = false;
var cullface = false;

// Create example material
var mat = new Material();
mat.ambient(.0, .6, .6, 1);
mat.diffuse(.0, .4, .8, 1);
mat.specular(.0, .8, .8, 1);
mat.emission(.0, .2, .3, 1);
mat.shininess(100);

var light = new Light();
light.ambient(0.5, 0.1, 0.5, 1);

// GL Draw loop
var loop = function(timePassed, mx, my){
  // Update mouse position
  mouse.x = mx;
  mouse.y = my;
  
  gl.clear( 0.1, 0.1, 0.1 );
  
  // Update rotation
  rotation.y += 0.6;
  rotation.z += 0.8;
  
  gl.pushMatrices();
  gl.enableDepthRead();
  
  if(wireframe) gl.enableWireframe();
  
  gl.enable(gl.LIGHTING);
  light.enable()

  gl.translate(mouse.x, mouse.y);
  gl.rotate(rotation.x, rotation.y, rotation.z);
  
  mat.apply();
  
  if(drawMethod == 0) {
    gl.drawTorus(60, 30, 16, 16);
  } else if(drawMethod == 1) {
    gl.drawCube(0, 0, 0, 60, 60, 60);
  } else if(drawMethod == 2) {
    gl.drawCylinder(60, 30, 50, 16, 4);
  } else if(drawMethod == 3) {
    gl.drawSphere(0, 0, 0, 30, 16);
  }
  
  light.disable();
  gl.disable(gl.LIGHTING);
  
  if(wireframe) gl.disableWireframe();

  gl.disableDepthRead();
  gl.popMatrices();
}

// Register draw loop (executed each frame, allows drawing to window)
app.draw(loop);

// Handle key events
app.on('keydown', function( evt ){
  if(evt.charCode == 99) { // C
    toggleAppConsole();
  } else if(evt.charCode == 27) { // ESC
    quit();
  } else if(evt.charCode == 118) { // V
    toggleV8Stats();
  } else if(evt.charCode == 119) { // W
    wireframe = !wireframe;
  } else if(evt.charCode == 114) { // R
    drawMethod++
    if(drawMethod > 3){
      drawMethod = 0
    }
  } else if(evt.charCode == 108) { // L
    
  } else if(evt.charCode == 102) { // F
    
  }

});