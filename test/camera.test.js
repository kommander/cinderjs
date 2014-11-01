var Camera = require('camera').Camera;
var assert = require('assert').assert.ok

var cam = new Camera();

// Check id non mutable
assert(cam.id == 1);
cam.id = 10;
assert(cam.id == 1);
