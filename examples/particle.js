var Vec2 = require('./lib/vec2.js').Vec2;

function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

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
  particle.rgb = [1,1,1];
  return particle;
}

removeParticles = function( amt ) {
  particles.splice(0, amt);
};

var particle;
var i;
var colorBuf = 0;
var distance;

// NOTE: Utility methods like these can be wrapped
//       in a convinience js object when modules have arrived...
var textId = utils.createSimpleText("Particles: 2000");
utils.setSimpleTextPosition(textId, 0, 40, 0);

var loop = function(timePassed){
  for( i = particles.length - 1; i >= 0; i-- ) {
    particle = particles[i];
    if(particle.x < 0 || particle.x > ctxSize.x || particle.y < 0 || particle.y > ctxSize.y){
      particle.reverse();
    }
    if(moveParticles) {
      distance = particle.distance(mouse)
      if(mouseActive && distance < 80) {
        particle.rgb[0] = 1 - 2 / distance;
        particle.rgb[1] = 1 - 1 / distance;
        particle.rgb[2] = 1 - .5 / distance;
        particle.angleTo(mouse);
        if(mouse.down) {
          particle.reverse();
        }
        particle.forward(particle.vel * 2 * timePassed / 40);
      } else {
        particle.forward(particle.vel * timePassed / 40);
      }
    }
    //gl.color(particle.rgb[0], particle.rgb[1], particle.rgb[2]);
    
    if(drawMethod == 0) {
      gl.drawSolidCircle( particle.x, particle.y, particle.radius );
    } else if(drawMethod == 1) {
      gl.drawLine(lastParticle.x, lastParticle.y, particle.x, particle.y);
      lastParticle = particle;
    } else if(drawMethod == 2) {
      gl.drawCube(particle.x, particle.y, 0, 20, 20, 20);
    }
  }
  
  // Draw the earlier created text object...
  utils.drawSimpleText(textId);
}

__draw__(function(timePassed, mx, my){
  // Update mouse position
  mouse.x = mx;
  mouse.y = my;
  
  loop(timePassed);
});

app.on('resize', function( width, height ){
  ctxSize.x = width;
  ctxSize.y = height;
});

app.on('keydown', function( evt ){
  if(evt.charCode == 99) { // C
    toggleAppConsole();
  } else if(evt.charCode == 109) { // M
    generateParticles(1000);
    utils.updateSimpleText(textId, "Particles: " + particles.length);
  } else if(evt.charCode == 108) { // L
    removeParticles(1000);
    utils.updateSimpleText(textId, "Particles: " + particles.length);
  } else if(evt.charCode == 114) { // R
    drawMethod++
    if(drawMethod > 2){
      drawMethod = 0
    }
  } else if(evt.charCode == 27) { // ESC
    quit();
  } else if(evt.charCode == 118) { // V
    toggleV8Stats();
  }

});

// Add some particles
generateParticles(2000);
