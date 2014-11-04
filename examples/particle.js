var text = require('text');
var gl = require('gl');
var Vec2 = require('./lib/vec2.js');
var Material = require('material');

// Screen size holder
var ctxSize = {
  x: 640,
  y: 480
};

var moveParticles = true;
var drawMethod = 1;
var lastParticle;
var mouse = new Vec2();
var mouseActive = true;
var vertSync = gl.isVerticalSyncEnabled();

var particles = [];
function generateParticles( amt ){
  for( var i = amt; i > 0; i-- ) {
    particles.push( newParticle() );
  }
  lastParticle = particles[0];
}

function newParticle(){
  var particle = new Vec2( getRandomInt(0, ctxSize.x), getRandomInt(0, ctxSize.y), getRandomInt(0, 360) );
  particle.radius = getRandomInt(2, 5);
  particle.vel = getRandomInt(1, 4);
  particle.rx = getRandomInt(1, 360);
  particle.ry = getRandomInt(1, 360);
  particle.rz = getRandomInt(1, 360);
  particle.rgb = [1,1,1];
  particle.inReach = false;
  return particle;
}

removeParticles = function( amt ) {
  particles.splice(0, amt);
};

var particle;
var i;
var colorBuf = 0;
var distance;
var cubeSize = 20;
var cubeSizeBuf = cubeSize;

// NOTE: Utility methods like these can be wrapped
//       in a convinience js object when modules have arrived...
var textId = text.createSimpleText("Particles: 2000");
text.setSimpleTextPosition(textId, 0, 40, 0);
var textId2 = text.createSimpleText("Vertical Sync: " + vertSync);
text.setSimpleTextPosition(textId2, 0, 60, 0);

// Create example material
var mat = new Material();
mat.ambient(.0, .6, .6, 1);
mat.diffuse(.0, .4, .8, 1);
mat.specular(.0, .8, .8, 1);
mat.emission(.0, .2, .3, 1);
mat.shininess(100);

var loop = function(timePassed, mx, my){
  // Update mouse position
  mouse.x = mx;
  mouse.y = my;

  gl.clear( 0.1, 0.1, 0.1 );
  
  gl.enable(gl.LIGHTING);
  gl.enable(gl.LIGHT0);

  mat.apply();

  for( i = particles.length - 1; i >= 0; i-- ) {
    particle = particles[i];
    if(particle.x < 0 || particle.x > ctxSize.x || particle.y < 0 || particle.y > ctxSize.y){
      particle.reverse();
    }
    if(moveParticles) {
      
      distance = particle.distance(mouse)
      if(!particle.inReach && mouseActive && distance < 80) {
        particle.radius /= 2;
        particle.inReach = true;
        particle.rgb[0] = 1 - 2 / distance;
        particle.rgb[1] = 1 - 1 / distance;
        particle.rgb[2] = 1 - .5 / distance;
        particle.angleTo(mouse);
        particle.reverse();
        particle.forward(particle.vel * 2 * timePassed / 40);
      } 

      if(distance >= 80) {
        particle.radius = cubeSize; 
        particle.inReach = false;
        particle.forward(particle.vel * timePassed / 40);
      }

    }
    //gl.color(particle.rgb[0], particle.rgb[1], particle.rgb[2]);
    
    
    if(drawMethod == 0) {
      gl.drawSolidCircle( particle.x, particle.y, particle.radius );
    } else if(drawMethod == 2) {
      gl.drawLine(lastParticle.x, lastParticle.y, particle.x, particle.y);
      lastParticle = particle;
    } else if(drawMethod == 1) {
      // Update rotation
      particle.rx += 0.2;
      particle.ry += 0.5;
      particle.rz += 0.6;
      
      gl.pushMatrices();
      gl.enableDepthRead();
      gl.translate(particle.x, particle.y);
      gl.rotate(particle.rx, particle.ry, particle.rz);
      gl.drawCube(0, 0, 0, particle.radius, particle.radius, particle.radius);
      gl.disableDepthRead();
      gl.popMatrices();
    }
  }
  
  gl.disable(gl.LIGHT0);
  gl.disable(gl.LIGHTING);
  
  // Draw the earlier created text object...
  text.drawSimpleText(textId);
  text.drawSimpleText(textId2);
}

// Register draw loop (executed each frame, allows drawing to window)
app.draw(loop);

app.on('resize', function( width, height ){
  ctxSize.x = width;
  ctxSize.y = height;
});

app.on('keydown', function( evt ){
  if(evt.charCode == 99) { // C
    toggleAppConsole();
  } else if(evt.charCode == 109) { // M
    generateParticles(1000);
    text.updateSimpleText(textId, "Particles: " + particles.length);
  } else if(evt.charCode == 108) { // L
    removeParticles(1000);
    text.updateSimpleText(textId, "Particles: " + particles.length);
  } else if(evt.charCode == 114) { // R
    drawMethod++
    if(drawMethod > 2){
      drawMethod = 0
    }
  } else if(evt.charCode == 27) { // ESC
    quit();
  } else if(evt.charCode == 118) { // V
    toggleV8Stats();
  } else if(evt.charCode == 115) { // S
    if(vertSync){
      gl.disableVerticalSync();
    } else {
      gl.enableVerticalSync();
    }
    vertSync = !vertSync;
    text.updateSimpleText(textId2, "Vertical Sync: " + vertSync);
  }
  
});

// Add some particles
generateParticles(2000);
