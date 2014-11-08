//
// FBO Basic Example
var gl = require('gl');
var Shader = require('shader');
var Camera = require('camera');
var Fbo = require('fbo');
var Format = Fbo.Format;
var glm = require('glm');
var Mat4 = glm.Mat4;

var screenSize = { w: 640, h: 480 };

var FBO_WIDTH = 256;
var FBO_HEIGHT = 256;

var cubeRotation = new Mat4();
var rotationMatrix = glm.rotate(0.06, 0.16666, 0.333333, 0.666666);
var format = new Format();

//format.setSamples( 4 ); // uncomment this to enable 4x antialiasing
var fbo = new Fbo( FBO_WIDTH, FBO_HEIGHT, format.depthTexture() );

var fboCam = new Camera( fbo.width, fbo.height, 60.0 );
fboCam.setPerspective( 60, fbo.getAspectRatio(), 1, 1000 );
fboCam.lookAt( 2.8, 1.8, -2.8, 0, 0, 0);

var cam = new Camera( screenSize.w, screenSize.h, 60.0 );
cam.setPerspective( 60, app.getAspectRatio(), 1, 1000 );
cam.lookAt( 2.6, 1.6, -2.6, 0, 0, 0 );

var fboShader = Shader.getStockColor();
var texShader = Shader.getStockTexture();

// Uncomment these lines if you are experiencing casual frame rate drops to 45 or 30 fps
app.disableFrameRate();
gl.enableVerticalSync();

function renderSceneToFbo(){
  fbo.bindBuffer();

  // clear out the FBO with blue
  gl.clear( 0.25, 0.5, 1.0 );

  // setup the viewport to match the dimensions of the FBO
  gl.pushViewport( 0, 0, fbo.width, fbo.height );

  gl.setMatrices( fboCam.id );

  // set the modelview matrix to reflect our current rotation
  cubeRotation.mult( rotationMatrix );
  gl.setModelMatrix( cubeRotation );
  
  // render the color cube
  fboShader.bind();

  gl.color( 1, 0.5, 0.25 );
  gl.drawColorCube( 0, 0, 0, 2.2, 2.2, 2.2 );
  gl.color( 1, 1, 1 );

  gl.popViewport();
  fbo.unbindBuffer();
}

// GL Draw loop
app.draw(function(timePassed, mx, my){
  gl.enableDepth();
  
  renderSceneToFbo();

  gl.clear( 0.35, 0.35, 0.35 );

  // setup our camera to render the cube
  gl.setMatrices( cam.id );

  // use the scene we rendered into the FBO as a texture
  fbo.bindTexture();

  // draw a cube textured with the FBO
  texShader.bind();
  gl.drawCube( 0, 0, 0, 2.2, 2.2, 2.2 );
  
  // show the FBO color texture in the upper left corner
  gl.setMatricesWindow( screenSize.w, screenSize.h );
  gl.drawTexture( fbo.getColorTexture(), 0, 0, 128, 128 );

  // and draw the depth texture adjacent
  gl.drawTexture( fbo.getDepthTexture(), 128, 0, 256, 128 );

  gl.disableDepth();
});

app.on('resize', function(w, h){
  screenSize.w = w;
  screenSize.h = h;

  cam.setPerspective( 60, app.getAspectRatio(), 1, 1000 );
});
