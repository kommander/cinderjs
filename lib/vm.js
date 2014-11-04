var vm = exports;
var self = this;

vm.runInThisContext = function( script, filename ) {
  return self._vm.runInThisContext( script, filename );
}
