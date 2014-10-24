//
// Cubes Example
var Material = require('material').Material;

// Screen size holder
var ctxSize = { x: 640, y: 480 };

// Mouse
var mouse = { x: 0, y: 0 };

// Rotation
var rotation = { x: 0, y: 0, z: 0 };

var lights = [gl.LIGHT0, gl.LIGHT1, gl.LIGHT2, gl.LIGHT3, gl.LIGHT4, gl.LIGHT5, gl.LIGHT6];
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

//gl.enable(gl.CULL_FACE);

// GL Draw loop
var loop = function(timePassed, mx, my){
  // Update mouse position
  mouse.x = mx;
  mouse.y = my;
  
  // Update rotation
  rotation.y += 0.6;
  rotation.z += 0.8;
  
  gl.pushMatrices();
  if(wireframe) gl.enableWireframe();
  
  gl.translate(mouse.x, mouse.y);
  gl.rotate(rotation.x, rotation.y, rotation.z);
  
  gl.enable(gl.LIGHTING);
  gl.enable(lights[currentLight]);

  mat.apply();
  
  if(drawMethod == 0) {
    gl.drawTorus(60, 30);
  } else if(drawMethod == 1) {
    gl.drawCube(0, 0, 0, 60, 60, 60);
  } else if(drawMethod == 2) {
    gl.drawCylinder(60, 30, 50, 16, 4);
  }
  
  gl.disable(lights[currentLight]);
  gl.disable(gl.LIGHTING);
  
  if(wireframe) gl.disableWireframe();
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
    if(drawMethod > 2){
      drawMethod = 0
    }
  } else if(evt.charCode == 108) { // L
    currentLight++
    if(currentLight > lights.length - 1){
      currentLight = 0
    }
  } else if(evt.charCode == 102) { // F
    
  }

});