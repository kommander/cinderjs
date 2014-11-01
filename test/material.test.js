var Material = require('material').Material;
var assert = require('assert').assert.ok

var mat = new Material();

// Check id non mutable
assert(mat.id == 1);
mat.id = 10;
assert(mat.id == 1);
