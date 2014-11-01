//
// Ray usage example
var Ray = require('ray').Ray;

// Screen Size
var ctxSize = { w: 640, h: 480 };

// Triangle Vectors
var v1 = { x: -50, y: -50, z: 0 };
var v2 = { x: 100, y: -50, z: 50 };
var v3 = { x: 75, y: 150, z: 0 };

var theRay = new Ray();
theRay.setDirection(0, 0, 0.000001);

log('Ray Example');

app.draw(function(timePassed, mx, my){

  gl.clear( 0.1, 0.1, 0.1 );

  gl.enableDepthRead();

  gl.pushMatrices();
  gl.translate(ctxSize.w / 2, ctxSize.h / 2, 0);
    
  theRay.setOrigin(mx - ctxSize.w / 2, my - ctxSize.h / 2, -100);

  gl.color(0.8, 0.8, 1);
  gl.begin(gl.TRIANGLES);
  gl.vertex(v1.x, v1.y, v1.z);
  gl.vertex(v2.x, v2.y, v2.z);
  gl.vertex(v3.x, v3.y, v3.z);
  gl.end();
  
  // Check intersection
  var result = theRay.calcTriangleIntersection( v1, v2, v3 );

  if(result){
    log('intersects');
    var pos = theRay.calcPosition(result);
    gl.color(0.9, 0.2, 0.4);
    gl.drawSphere(mx - ctxSize.w / 2, my - ctxSize.h / 2, pos.z, 20, 16);
  } 

  gl.popMatrices();
  gl.disableDepthRead();
});

app.on('resize', function(w, h){
  ctxSize.w = w;
  ctxSize.h = h;
});