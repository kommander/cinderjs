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

#include "app.hpp"
#include "AppConsole.h"

using namespace std;
using namespace cinder;
using namespace v8;

namespace cjs {

/**
 *
 */
void AppModule::getAspectRatio(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  
  float ratio = getApp()->getWindowAspectRatio();
  
  args.GetReturnValue().Set(v8::Number::New(isolate, ratio));
  return;
}

/**
 *
 */
void AppModule::addAssetDirectory(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  
  if(args[0]->IsString()){
    v8::String::Utf8Value pathStr(args[0]->ToString());
    getApp()->addAssetDirectory(fs::path(*pathStr));
  }
  
  return;
}

/**
 *
 */
void AppModule::disableFrameRate(const v8::FunctionCallbackInfo<v8::Value>& args) {
  
  getApp()->disableFrameRate();
  
  return;
}

/**
 *
 */
void AppModule::setFrameRate(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  
  if(args[0]->IsNumber()){
    getApp()->setFrameRate(args[0]->ToNumber()->Value());
  }
  
  return;
}


/**
 * Load bindings onto global js object
 */
void AppModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {

  // Create global app object
  Handle<ObjectTemplate> appTemplate = ObjectTemplate::New(getIsolate());
  
  appTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "getAspectRatio"), v8::FunctionTemplate::New(getIsolate(), getAspectRatio));
  appTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "addAssetDirectory"), v8::FunctionTemplate::New(getIsolate(), addAssetDirectory));
  appTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "disableFrameRate"), v8::FunctionTemplate::New(getIsolate(), disableFrameRate));
  appTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setFrameRate"), v8::FunctionTemplate::New(getIsolate(), setFrameRate));
  
  // Expose global app object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "app"), appTemplate);
}

 
} // namespace cjs