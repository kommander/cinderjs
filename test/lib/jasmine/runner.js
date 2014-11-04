var fs = require('fs');
var vm = require('vm');
var util = require('util');
var jasmine = require('./jasmine.js');
var ConsoleReporter = require('./console.js').ConsoleReporter();

var j = jasmine.core(jasmine);
var env = j.getEnv({
  global: global
});
var jasmineInterface = jasmine.interface(j, env);

for(k in jasmineInterface){
  global[k] = jasmineInterface[k];    
}

var cReporter = new ConsoleReporter({
  print: console.log
});

env.addReporter(jasmineInterface.jsApiReporter);
env.addReporter(cReporter);

// TODO: take argv folder, require() all modules that match *.test.js or *.spec.js

env.execute();
