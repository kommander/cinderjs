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
//var glslProg = new Shader( 'basic.vert', 'basic.frag', 'basic.geom' );

// setup VertBatch with a single point at the origin
var batch = new VertBatch();
batch.vertex( 0, 0 );  
batch.color( 1, 0, 0 );

// Uncomment these lines if you are experiencing casual frame rate drops to 45 or 30 fps
// app.disableFrameRate();
// gl.enableVerticalSync();

// GL Draw loop
var loop = function(timePassed, mx, my){
  gl.enableDepthRead();
  gl.enableDepthWrite();

  gl.clear( 0.1, 0.1, 0.11 );
  
  gl.setMatricesWindow(screenSize.w, screenSize.h);
  gl.translate(screenSize.w / 2, screenSize.h / 2);

  gl.scopedShader( glslProg );
  glslProg.uniformInt( "uNumSides", numSides );
  glslProg.uniformFloat( "uRadius", radius ); 
  
  batch.draw();

  gl.disableDepthRead();
  gl.disableDepthWrite();
};

// Register draw loop (executed each frame, allows drawing to window)
app.draw(loop);

app.on('resize', function(w, h){
  screenSize.w = w;
  screenSize.h = h;
});
