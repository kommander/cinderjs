function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

// Screen size holder
var ctxSize = {
  x: 640,
  y: 480
};

var loop = function(){
  for(var i = 0; i < 1000; i++){
    gl.drawLine(
      getRandomInt(0, ctxSize.x), getRandomInt(0, ctxSize.y),
      getRandomInt(0, ctxSize.x), getRandomInt(0, ctxSize.y)
    );
  }
}

app.draw(function(){
  loop();
});