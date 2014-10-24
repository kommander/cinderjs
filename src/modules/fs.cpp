/*
 Copyright (c) Sebastian Herrlinger - All rights reserved.
 This code is intended for use with the Cinder C++ library: http://libcinder.org
 
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
 */

#include "fs.hpp"
#include "AppConsole.h"

#include <string.h>
#include <fstream>
#include <sstream>
#include <cerrno>

#include "cinder/Filesystem.h"

using namespace std;
using namespace cinder;
using namespace v8;

namespace cjs {

void existsSync(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  
  String::Utf8Value path(args[0]->ToString());
  
  bool exists = fs::exists(boost::filesystem::path(*path));
  
  args.GetReturnValue().Set(v8::Boolean::New(isolate, exists));
  
  return;
}

void statSync(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  
  String::Utf8Value path(args[0]->ToString());
  
  boost::filesystem::file_status rawStats = fs::status(boost::filesystem::path(*path));
  
  Local<Object> statObj = Object::New(isolate);
  
  // Check dir
  bool isDir = false;
  if(rawStats.type() == boost::filesystem::directory_file){
    isDir = true;
  }
  statObj->Set(v8::String::NewFromUtf8(isolate, "isDirectory"), v8::Boolean::New(isolate, isDir));
  
  // Check symbolic link
  bool isSymbolic = false;
  if(rawStats.type() == boost::filesystem::symlink_file){
    isSymbolic = true;
  }
  statObj->Set(v8::String::NewFromUtf8(isolate, "isSymbolicLink"), v8::Boolean::New(isolate, isSymbolic));
  
  args.GetReturnValue().Set(statObj);
  
  return;
}

void readFileSync(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  
  String::Utf8Value path(args[0]->ToString());
  
  // Do we have a js file to run?
  std::string jsFileContents;
  if( !cinder::fs::exists( fs::path::path(*path) ) ){
    std::string err = "File not found: ";
    err.append(*path);
    isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, err.c_str())));
    return;
  } else {
    try {
      std::ifstream in(*path, std::ios::in );
      if (in)
      {
        std::ostringstream contents;
        contents << in.rdbuf();
        jsFileContents = contents.str();
        in.close();
      }
    } catch(std::exception &e) {
      std::string err = "Error: ";
      err.append(e.what());
      isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, err.c_str())));
      return;
    }
  }
  
  args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, jsFileContents.c_str()));
  
  return;
}

/**
 * Add JS bindings
 */
void FSModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global fs object
  Handle<ObjectTemplate> fsTemplate = ObjectTemplate::New(getIsolate());
  
  fsTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "existsSync"), v8::FunctionTemplate::New(getIsolate(), existsSync));
  fsTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "readFileSync"), v8::FunctionTemplate::New(getIsolate(), readFileSync));
  fsTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "statSync"), v8::FunctionTemplate::New(getIsolate(), statSync));
  
  // Expose global fs object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "_fs"), fsTemplate);
}
 
} // namespace cjs