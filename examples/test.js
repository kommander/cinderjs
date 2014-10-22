log("External World!" + Math.random());

var line = [{
  x: 0,
  y: 0
},
{
  x: 400,
  y: 400
}];

var clear = function(){
  log("clear");
}

__draw__(function(){
  clear();
  //log("Drawing from JS...");
  //glDrawLine(line[0], line[1]);
  glDrawSolidCircle(line[1], 30);
});

// TODO: implement setTimeout
//setTimeout(function(){
//  log("Timed out...");
//}, 2000);