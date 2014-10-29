var Material = require('material').Material;
var timers = require('timers');

var mat;

// Check a lot of weak garbage
// for(var i = 0; i < 100000; i++){
// 	mat = null;
// 	mat = new Material();
// }

// Test late dereference
mat = new Material();
timers.setTimeout(function(){
	log('Unreferencing material.');
	mat.destroy();
}, 10000);

for(var i = 0; i < 15000; i++){
	var g = { some: "garbage " };
}