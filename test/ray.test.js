var Ray = require('ray').Ray;
var assert = require('assert').assert.ok

var ray = new Ray();

// Check id non mutable
assert(ray.id == 1);
ray.id = 10;
assert(ray.id == 1);
