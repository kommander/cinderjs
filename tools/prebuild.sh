#!/bin/sh
python ../tools/js2c.py     \
  ../src/js_natives.h       \
  ../lib/cinder.js          \
  ../lib/path.js            \
  ../lib/util.js            \
  ../lib/module.js          \
  ../lib/fs.js              \
  ../lib/vm.js              \
  ../lib/assert.js          \