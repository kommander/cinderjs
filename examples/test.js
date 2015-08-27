console.log("running test...");

// Oh, how I wish I had a testsuite running already...

// Run a lot of timeouts at the same time
for(var i = 0; i < 100; i++){
  setTimeout(function(now){
    console.log("Timed out in " + (Date.now() - now) + "ms");
  }, 2000, Date.now());
}

//function doTimeout(){
//  setTimeout(function(now){
//    console.log("Timed out in " + (Date.now() - now) + "ms");
//    doTimeout();
//  }, 100, Date.now());
//}
//doTimeout();
//
// Test clearing a timer
var timer1 = setTimeout(function(){
  console.log('Error: We should not get here, as the timeout was canceled');
}, 900);

setTimeout(function(){
  console.log('Fine, this one was not cleared');
}, 1100);

setTimeout(function(){
  console.log('Clearing timer...');
  clearTimeout(timer1);
}, 500);

// Test interval
var timer = setInterval(function(now){
  console.log("Interval Time passed: " + (Date.now() - now) + "ms");
}, 100, Date.now());

setTimeout(function(){
  console.log('Clearing interval...');
  clearInterval(timer);
}, 450);