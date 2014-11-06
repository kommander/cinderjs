//
// Geometry Shader Example
var gl = require('gl');
var Shader = require('shader');
var Format = Shader.Format;
var VertBatch = require('batch').VertBatch;

app.addAssetDirectory(__dirname + '/assets/');

var screenSize = { w: 640, h: 480 };
var numSides = 4;
var radius = 100.0;

var format = new Format();
var glslProg = new Shader( format.vertex('basic.vert').fragment('basic.frag').geometry('basic.geom') );

// setup VertBatch with a single point at the origin
var batch = new VertBatch();
batch.vertex( 0, 0 );  
batch.color( 1, 0, 0 );

// Uncomment these lines if you are experiencing casual frame rate drops to 45 or 30 fps
app.disableFrameRate();
gl.enableVerticalSync();

// GL Draw loop
app.draw(function(timePassed, mx, my){
  gl.enableDepthRead();
  gl.enableDepthWrite();

  // Update
  numSides = mx / screenSize.w * 30 + 3;
  numSides = constrain( numSides, 2, 64 );
  radius = my / screenSize.h * ( screenSize.w / 2 );
  radius = constrain( radius, 1, screenSize.w / 2 );

  gl.clear( 0.1, 0.1, 0.11 );
  
  gl.setMatricesWindow(screenSize.w, screenSize.h);
  gl.translate(screenSize.w / 2, screenSize.h / 2);

  glslProg.bind();
  glslProg.uniformInt( "uNumSides", numSides );
  glslProg.uniformFloat( "uRadius", radius ); 
  
  batch.draw();

  gl.disableDepthRead();
  gl.disableDepthWrite();
});

app.on('resize', function(w, h){
  screenSize.w = w;
  screenSize.h = h;
});
