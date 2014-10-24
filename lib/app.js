//
// App
// Bootstrapping the global.app namespace with application features,
// such as encapsulating raw events from Cinder

var Internal = function(){};

Internal.handleRawEvent = function( type ){
  // Resize Event
  if(type == 10){
    app.emit('resize', arguments[1], arguments[2]);
  }
  // Key Down
  else if(type == 20){
    // TODO: Make real key event with goodies
    app.emit('keydown', {
      charCode: arguments[1],
      char: arguments[2]
    });
  }
  // Key Up
  else if(type == 30){
    // TODO: Make real key event with goodies
    app.emit('keyup', {
      charCode: arguments[1],
      char: arguments[2]
    });
  }

};

var NATIVE_DRAW = __draw__;
delete __draw__;
app.draw = function(loop){
  NATIVE_DRAW(loop);
};

// Setup event handler and remove raw from global
__event__(Internal.handleRawEvent);
delete __event__;

//
// Bootstrap helper
getRandomInt = function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}
