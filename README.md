# Cinder.js
OpenGL + Javascript

# Intentions
In an effort to conceptualize a programming course for beginning minors and children,
it became clear in early tests that fast visual feedback is the best motivation.
Development basics like low level networking and direct file access are not possible in the browser. 
Back in the days an editor and a compiler would be enough
to start out with simple command line programs and standard input/output.
Bringing basics of web development to beginners involves at least three languages: HTML, CSS, Javascript.
Too much overhead to draw a line on the screen.
So while maintainig a feature rich foundation for graphical scripting, 
it is supposed to be the base for a simplified gfx programming abstraction, suitable for children and minors,
with abilities to exchange cross platform compatible scripts.

# The Rough Roadmap
- Get a JS test suite up and running to stabilize API early on
- More GL features!
- Add a usefull 2D REPL console
- Security features (like user dialogues for file and network access etc.)
- Allow remote resource loading (http://mydomain.org/index.cjs, certificates, user trusted domains, domain specific contexts)
- Native modules (dynamically load additional bindings)
- Multi Window/Display support

# Run
## OSX
Drag a js file onto the application. If it is a cinder module, it will run it. If another script is already running,
it is shut down and replaced.  
Or, simply specify a "*.js" file to run on startup:  
```
$> open xcode/build/Debug/cinderjs.app --args /examples/cube/cubes.js
```

## Hotkeys
- __ESC 2x__  
Hitting _ESC_ two times fast will first exit fullscreen mode and if not in fullscreen mode,
it will quit and terminate the application completely.
- __F1__  
Toggle FPS text, showing the current Cinder & v8 frame rate.
- __F2__  
Toggle v8 heap statistics.
- __F3__  
Toggle textual console overlay.

# Build & Develop
Checkout the git submodules with:
```
$> git submodule init
$> git submodule update
```
Then build v8 (make x64.debug || x64.release) and Cinder (./xcode/fullbuild.sh). 
After that the xcode cinderjs project should build as alls paths are relative. 

## v8
Using Version 4.7.3

Get [depot_tools](http://www.chromium.org/developers/how-tos/install-depot-tools) and add to your PATH:
```
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH=`pwd`/depot_tools:"$PATH"
```
Then run
```
fetch v8
cd v8
git checkout 4.7.3
gclient sync
make x64.debug
```


To be able to debug JIT code, build v8 with the compiler flag "gdbjit=on".
For detailed build instructions have a look at [the Google V8 build docs](https://developers.google.com/v8/build).

To be able to link against libc++ with clang, build on OSX with:  
```
export GYP_DEFINES="clang=1 host_clang=1 use_custom_libcxx=1 mac_deployment_target=10.10 clang_dir=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr"
```

Make example using 8 parallel build processes:
```
make -j8 -e x64.debug
```

## Cinder
To avoid building all the libs within "fullbuild.sh":
```
xcrun xcodebuild -project Cinder/xcode/cinder.xcodeproj -target cinder -configuration Debug
```

### License

Copyright (c) Sebastian Herrlinger - All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and
the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.