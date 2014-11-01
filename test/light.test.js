var Light = require('light').Light;
var assert = require('assert').assert.ok

var light = new Light();

// Check id non mutable
assert(light.id == 1);
light.id = 10;
assert(light.id == 1);
