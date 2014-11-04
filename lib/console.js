var util = require('util');
var self = this;

exports.log = function(){
  var lines = [];
  var output = '';
  for(var i = 0; i < arguments.length; i++){
    if(util.isUndefined(arguments[i])) arguments[i] = '[undefined]';
    var str = arguments[i].toString();
    output += ' ' + str;
  }
  var parts = output.split('\n');
  for(var j = 0; j < parts.length; j++){
    self.log(parts[j]);
  }
}