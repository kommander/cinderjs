//
//  Derived from Cinder Sample by David Wicks (sansumbrella.com)
//

var gl = require('gl');
var Batch = require('batch');
var Shader = require('shader');
var Format = Shader.Format;
var cm = require('color');
var lmap = require('math').lmap;
var Vbo = require('vbo');
var Vao = require('vao');
var glm = require('glm');
Vec3 = glm.Vec3;

app.addAssetDirectory(__dirname + '/assets/');

var screenSize = { w: 640, h: 480 };

var NUM_PARTICLES = 100000;

// Descriptions of particle data layout.
var attributes = [];
// Buffers holding raw particle data on GPU.
var particleBuffer = [];

// Current source and destination buffers for transform feedback.
// Source and destination are swapped each frame after update.
var sourceIndex = 0;
var destinationIndex = 1;

// Mouse state suitable for passing as uniforms to update program
var mouseDown = false;
var mouseForce = 0.0;
var mousePos = Vec3();

// Create initial particle layout.
var particleAttribSize = 3 + 3 + 3 + 4 + 1;
var particleByteSize = particleAttribSize * 4;
var particles = new Float32Array( particleByteSize * NUM_PARTICLES );
var azimuth = 256 * Math.PI / NUM_PARTICLES;
var inclination = Math.PI / NUM_PARTICLES;
var radius = 180;
var center = Vec3( screenSize.w / 2, screenSize.h / 2 + 40,  0 );

for( var i = 0; i < NUM_PARTICLES; ++i ) { 
  var pos = i * particleAttribSize;
  
  // assign starting values to particles.
  var x = center.x + radius * Math.sin( inclination * i ) * Math.cos( azimuth * i );
  var y = center.y + radius * Math.cos( inclination * i );
  var z = center.z + radius * Math.sin( inclination * i ) * Math.sin( azimuth * i );

  // particle position vector
  particles[pos + 0] = x;
  particles[pos + 1] = y;
  particles[pos + 2] = z;

  // Color
  particles[pos + 3] = lmap( i, 0.0, NUM_PARTICLES, 0.0, 0.66 );
  particles[pos + 4] = 1;
  particles[pos + 5] = 1;
  particles[pos + 6] = 1;

  // Initial velocity
  particles[pos + 7] = x + Math.random() * 10;
  particles[pos + 8] = y + Math.random() * 10;
  particles[pos + 9] = z + Math.random() * 10;

  // particle home vector
  particles[pos + 10] = x;
  particles[pos + 11] = y;
  particles[pos + 12] = z;

  // Damping
  particles[pos + 13] = 0.965; // TODO: use random float 0.965 - 0.985
}

// Create particle buffers on GPU and copy data into the first buffer.
// Mark as static since we only write from the CPU once.
particleBuffer[sourceIndex] = Vbo( gl.ARRAY_BUFFER, particles, gl.STATIC_DRAW );
particleBuffer[destinationIndex] = Vbo( gl.ARRAY_BUFFER, particles.byteLength, gl.STATIC_DRAW );

// Create a default color shader.
var renderProg = Shader.getStockColor();

for( var i = 0; i < 2; ++i )
{ // Describe the particle layout for OpenGL.
  attributes[i] = Vao();
  attributes[i].bind();

  // Define attributes as offsets into the bound particle buffer
  particleBuffer[i].bind();

  gl.enableVertexAttribArray( 0 );
  gl.enableVertexAttribArray( 1 );
  gl.enableVertexAttribArray( 2 );
  gl.enableVertexAttribArray( 3 );
  gl.enableVertexAttribArray( 4 );
  gl.vertexAttribPointer( 0, 3, gl.FLOAT, gl.FALSE, particleByteSize, 0 );
  gl.vertexAttribPointer( 1, 4, gl.FLOAT, gl.FALSE, particleByteSize, 3 );
  gl.vertexAttribPointer( 2, 3, gl.FLOAT, gl.FALSE, particleByteSize, 7 );
  gl.vertexAttribPointer( 3, 3, gl.FLOAT, gl.FALSE, particleByteSize, 10 );
  gl.vertexAttribPointer( 4, 1, gl.FLOAT, gl.FALSE, particleByteSize, 13 );

  particleBuffer[i].unbind();
  attributes[i].unbind();
}

// Load our update program.
// Match up our attribute locations with the description we gave.
var format = new Format();
var updateProg = new Shader( 
  format.vertex('particleUpdate.vs')
    .feedbackFormat( gl.INTERLEAVED_ATTRIBS )
    .feedbackVaryings( ["position", "pposition", "home", "color", "damping"] )
    .attribLocation( "iPosition", 0 )
    .attribLocation( "iColor", 1 )
    .attribLocation( "iPPosition", 2 )
    .attribLocation( "iHome", 3 )
    .attribLocation( "iDamping", 4 )
);

// Listen to mouse events so we can send data as uniforms.
app.on('mousedown', function(){
  mouseDown = true;
  mouseForce = 500;
});

app.on('mouseup', function(){
  mouseForce = 0;
  mouseDown = false;
});

app.disableFrameRate();
gl.enableVerticalSync();

gl.enableDepth();
  
app.draw(function(timePassed, mx, my){

  // Update mouse position for uniform
  if(mouseDown){
    mousePos.set(mx, my, 0);
  }

  //Update particles on the GPU
  //updateProg.bind();
  gl.pushShader(updateProg.id);
  gl.pushBoolState( gl.RASTERIZER_DISCARD, true );  // turn off fragment stage
  updateProg.uniformFloat( "uMouseForce", mouseForce );
  updateProg.uniformVec3( "uMousePos", mousePos );

  // Bind the source data (Attributes refer to specific buffers).
  //attributes[sourceIndex].bind();
  gl.pushVao(attributes[sourceIndex].id);

  // Bind destination as buffer base.
  gl.bindBufferBase( gl.TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffer[destinationIndex] );
  gl.beginTransformFeedback( gl.POINTS );

  // Draw source into destination, performing our vertex transformations.
  gl.drawArrays( gl.POINTS, 0, NUM_PARTICLES );

  gl.endTransformFeedback();
  //attributes[sourceIndex].unbind();
  gl.popVao();
  gl.popShader();
  
  // Swap source and destination for next loop
  var buf = sourceIndex;
  sourceIndex = destinationIndex;
  destinationIndex = buf;

  // Update mouse force.
  if( mouseDown ) {
    mouseForce = 150.0;
  }

  // Activate rasterizer again
  gl.popBoolState( gl.RASTERIZER_DISCARD );

  gl.clear( 0, 0, 0 );
  gl.setMatricesWindowPersp( screenSize.w, screenSize.h );
  gl.enableDepth();
  
  gl.pushShader(renderProg.id);
  //renderProg.bind();
  //attributes[sourceIndex].bind();
  gl.pushVao(attributes[sourceIndex].id);
  gl.setDefaultShaderVars();
  gl.drawArrays( gl.POINTS, 0, NUM_PARTICLES );
  
  //attributes[sourceIndex].unbind();
  gl.popVao();
  
  gl.disableDepth();
  gl.popShader();

});

