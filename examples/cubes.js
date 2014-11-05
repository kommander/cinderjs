//
// Cubes Example
var gl = require('gl');
var Camera = require('camera');
var Shader = require('shader');
var Batch = require('batch');
var Texture = require('texture');
var glm = require('glm');
var Mat4 = glm.Mat4;

app.addAssetDirectory(__dirname + '/assets/');

// Screen size holder
var ctxSize = { x: 640, y: 480 };

var wireframe = false;

var cam = new Camera();
cam.lookAt( 3, 2, 4, 0, 0, 0 );
cam.setPerspective( 60.0, app.getAspectRatio(), 1.0, 1000.0 );

var texFormat = null; // TODO: implement
var texture = new Texture( 'texture.jpg', texFormat );

var glslProg = new Shader( 'shader.vert', 'shader.frag' );
var batch = new Batch(0, glslProg);

// Setup rotation matrices
var cubeRotation = new Mat4();
var rotationMatrix = glm.rotate(0.01, 0, 1, 0);

// Uncomment these lines if you are experiencing casual frame rate drops to 45 or 30 fps
app.disableFrameRate();
gl.enableVerticalSync();

// GL Draw loop
var loop = function(timePassed, mx, my){
  gl.enableDepthRead();
  gl.enableDepthWrite();

  if(wireframe) gl.enableWireframe();

  gl.clear( 0.1, 0.1, 0.11 );
  
  // Update rotation
  cubeRotation.mult(rotationMatrix);
  
  gl.setMatrices(cam.id);
  gl.pushMatrices();
    gl.multModelMatrix(cubeRotation.id); // TODO: missing a wrapper for multModelMatrix to take Mat4 objects directly

    texture.bind();
    batch.draw();

  gl.popMatrices();

  if(wireframe) gl.disableWireframe();

  gl.disableDepthRead();
  gl.disableDepthWrite();
};

// Register draw loop (executed each frame, allows drawing to window)
app.draw(loop);

app.on('resize', function(w, h){
  ctxSize.x = w;
  ctxSize.y = h;

  cam.setPerspective( 60.0, app.getAspectRatio(), 1.0, 1000.0 );
});

app.on('keydown', function( evt ){

  if(evt.charCode == 99) { // C
    toggleAppConsole();
  } else if(evt.charCode == 27) { // ESC
    quit();
  } else if(evt.charCode == 118) { // V
    toggleV8Stats();
  } else if(evt.charCode == 119) { // W
    wireframe = !wireframe;
  }

});