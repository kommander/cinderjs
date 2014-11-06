#!/bin/sh
rm ../src/js_natives.h  
python ../tools/js2c.py     \
  ../src/js_natives.h       \
  ../lib/cinder.js          \
  ../lib/path.js            \
  ../lib/util.js            \
  ../lib/module.js          \
  ../lib/fs.js              \
  ../lib/vm.js              \
  ../lib/assert.js          \
  ../lib/timers.js          \
  ../lib/events.js          \
  ../lib/app.js             \
  ../lib/color.js           \
  ../lib/ray.js             \
  ../lib/camera.js          \
  ../lib/gl.js              \
  ../lib/text.js            \
  ../lib/console.js         \
  ../lib/shader.js          \
  ../lib/batch.js           \
  ../lib/texture.js         \
  ../lib/glm.js             \
  ../lib/default_main.js    \