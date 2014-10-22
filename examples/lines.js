log('>>> LINES.JS START <<<');

function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

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

  __draw__(function(){
    loop();
  });
} catch(ex) {
  log("Error: " + ex);
  return;
}

log('>>> LINES.JS END <<<');
