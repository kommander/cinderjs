//
// Some Math additions from Cinder

exports.lmap = function( val, inMin, inMax, outMin, outMax ){
  return outMin + (outMax - outMin) * ((val - inMin) / (inMax - inMin));
}

