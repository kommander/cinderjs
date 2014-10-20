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
  
v8::Persistent<v8::Function> AppModule::sDrawCallback;
v8::Persistent<v8::Function> AppModule::sEventCallback;

/**
 * Draw
 */
void AppModule::draw(){
  v8::Locker lock(getIsolate());

  // Isolate
  v8::Isolate::Scope isolate_scope(getIsolate());
  v8::HandleScope handleScope(getIsolate());
  
  // Callback
  v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(getIsolate(), sDrawCallback);
  
  v8::Local<v8::Context> context = v8::Local<v8::Context>::New(getIsolate(), *getContext());
  
  if(context.IsEmpty()) return;
  
  Context::Scope ctxScope(context);
  context->Enter();
  
  if( !callback.IsEmpty() ){
    // TODO: call draw callback in v8 context
    v8::Handle<v8::Value> argv[0] = {};
    
    callback->Call(context->Global(), 0, argv);
    
    callback.Clear();
    argv->Clear();
  }
  
  context->Exit();
}

/**
 * Set the draw callback from javascript
 */
void AppModule::drawCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Locker lock(isolate);
  v8::HandleScope handleScope(isolate);
  
  if(!args[0]->IsFunction()){
    // throw js exception
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "draw callback expects one argument of type function."));
    return;
  }
  AppConsole::log("draw callback set.");
  
  sDrawCallback.Reset(isolate, args[0].As<v8::Function>());
  
  // TODO: strip drawCallback from global obj, so only one main loop can be used
  
  return;
}

/**
 * Set event callback from javascript to push mouse/key events to
 */
void AppModule::rawEventCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Locker lock(isolate);
  v8::HandleScope handleScope(isolate);
  
  if(!args[0]->IsFunction()){
    // throw js exception
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "event callback expects one argument of type function."));
    return;
  }
  AppConsole::log("event callback set.");
  
  sEventCallback.Reset(isolate, args[0].As<v8::Function>());
  
  // TODO: Remove event callback setter function from global context
  
  return;
}

/**
 * Handle a mouse move event on the application and push it to v8
 */
void AppModule::mouseMove(cinder::app::MouseEvent evt){
  v8::Locker lock(getIsolate());

  // Isolate
  v8::Isolate::Scope isolate_scope(getIsolate());
  v8::HandleScope handleScope(getIsolate());
  
  // Callback
  v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(getIsolate(), sEventCallback);
  
  if( !callback.IsEmpty() ) {
    
    v8::Handle<v8::Value> argv[3] = {
      v8::Number::New(getIsolate(), 1),
      v8::Number::New(getIsolate(), evt.getX()),
      v8::Number::New(getIsolate(), evt.getY())
    };
    
    callback->Call(callback->CreationContext()->Global(), 3, argv);
  }
  
  v8::Unlocker unlock(getIsolate());
}

/**
 * Load bindings onto global js object
 */
void AppModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global gl object
  Handle<ObjectTemplate> appTemplate = ObjectTemplate::New(getIsolate());
  
  // gl methods
  appTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "draw"), v8::FunctionTemplate::New(getIsolate(), drawCallback));
  appTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "rawEvent"), v8::FunctionTemplate::New(getIsolate(), rawEventCallback));
  
  // Expose global gl object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "app"), appTemplate);
}

 
} // namespace cjs