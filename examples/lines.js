log('>>> LINES.JS START <<<');

// Screen size holder
var ctxSize = {
  x: 640,
  y: 480
};

try {

  var loop = function(){
    for(var i = 0; i < 10000; i++){
      gl.drawLine(
        getRandomInt(0, ctxSize.x), getRandomInt(0, ctxSize.y),
        getRandomInt(0, ctxSize.x), getRandomInt(0, ctxSize.y)
      );
    }
  }

  // Register draw loop (executed each frame, allows drawing to window)
  app.draw(loop);

} catch(ex) {
  log("Error: " + ex);
  return;
}

log('>>> LINES.JS END <<<');
