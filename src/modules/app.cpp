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

namespace cjs {
  
v8::Persistent<v8::Function> AppModule::sDrawCallback;

void AppModule::draw(){
  
  // Isolate
  v8::Isolate::Scope isolate_scope(getIsolate());
  v8::HandleScope handleScope(getIsolate());
  
//  // Context
//  v8::Local<v8::Context> context = v8::Local<v8::Context>::New(getIsolate(), *getContext());
//  
//  if(context.IsEmpty()){
//    AppConsole::log("Context is empty");
//    return;
//  }
//  
//  v8::Context::Scope ctxScope(context);
//  
//  // Global
//  v8::Handle<v8::Object> global = context->Global();
//  
//  if(global.IsEmpty()){
//    AppConsole::log("Global is empty.");
//    return;
//  }

  
  // Callback
  v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(getIsolate(), sDrawCallback);
  
  if( !callback.IsEmpty() ){
    // TODO: call draw callback in v8 context
    v8::Handle<v8::Value> argv[0] = {};
    
    callback->Call(callback->CreationContext()->Global(), 0, argv);
  }
}

void AppModule::drawCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::EscapableHandleScope handleScope(isolate);
  
  if(!args[0]->IsFunction()){
    // TODO: throw js exception
    AppConsole::log("draw callback expects one argument of type function.");
    return;
  }
  AppConsole::log("draw callback set.");
  
  sDrawCallback.Reset(isolate, args[0].As<v8::Function>());
  
  return;
}

void AppModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  global->Set(v8::String::NewFromUtf8(getIsolate(), "draw"), v8::FunctionTemplate::New(getIsolate(), drawCallback));
}

 
} // namespace cjs