# CinderJS

# Intentions
In an effort to conceptualize a programming course for beginning minors and children,
it became clear in early tests that fast visual feedback is the best motivation.
Development basics like low level networking and direct file access are not possible in the browser. 
Back in the days an editor and a compiler would be enough
to start out with simple command line programs and standard input/output.
Bringing basics of web development to beginners involves at least three languages: HTML, CSS, Javascript.
Too much overhead to draw a line on the screen.
So while maintainig a feature rich foundation for graphical scripting, 
it is supposed to be the foundation for a simplified gfx programming abstraction, suitable for children and minors,
with abilities to exchange cross platform compatible scripts.

# Run
# OSX
Drag a js file onto the application. If it is a cinder module, it will run it. If another script is already running,
it is shut down and replaced.
Simply specify a "*.js" file to run on startup:  
```
$> open xcode/build/Debug/cinderjs.app --args /Users/sebastian/Dropbox/+Projects/cinderjs/lib/test.js
```

# Build & Develop
## v8
Using Version branch 3.30

To be able to debug JIT code, build v8 with the compiler flag "gdbjit=on".
For detailed build instructions have a look at [the Google V8 build docs](https://developers.google.com/v8/build).

To be able to link against lib++, build on OSX with:  
```
export CXX="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -std=c++11 -stdlib=libc++"
export CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
export CPP="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -E"
export LINK="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -std=c++11 -stdlib=libc++"
export CXX_host=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++
export CC_host=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
export CPP_host="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -E"
export LINK_host=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++
export GYP_DEFINES="clang=1 mac_deployment_target=10.8"
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