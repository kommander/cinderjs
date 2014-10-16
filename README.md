# CinderJS

# Run
# OSX
Simply specify a .js file to run on startup:
$> open xcode/build/Debug/cinderjs.app --args /Users/sebastian/Dropbox/+Projects/cinderjs/lib/test.js

# Build
## v8
Using Version branch 3.29

To be able to link against lib++, build on OSX with:

  export CXX="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -std=c++11 -stdlib=libc++"
  export CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
  export CPP="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -E"
  export LINK="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -std=c++11 -stdlib=libc++"
  export CXX_host=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++
  export CC_host=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
  export CPP_host="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -E"
  export LINK_host=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++
  export GYP_DEFINES="clang=1 mac_deployment_target=10.8"
