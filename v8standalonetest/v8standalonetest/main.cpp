//
//  main.cpp
//  v8standalonetest
//
//  Created by Sebastian Herrlinger on 17/10/14.
//  Copyright (c) 2014 kosmokommando. All rights reserved.
//

#include <iostream>
#include "v8.h"

v8::Handle<v8::Context> ctx;
v8::Isolate* isolate;
v8::Handle<v8::Function> sDrawCallback;

void draw(){
  v8::V8::Initialize();
  v8::HandleScope handleScope(isolate);
  
  //v8::Local<v8::Context> context = getContext();
  v8::Local<v8::Context> context = sDrawCallback->CreationContext();
  
  if(context.IsEmpty()){
    //AppConsole::log("Context is empty");
    return;
  }
  
  //v8::Context::Scope ctxScope(context);
  
  if( !sDrawCallback.IsEmpty() ){
    // TODO: call draw callback in v8 context
    v8::Handle<v8::Value> argv[0] = {};
    sDrawCallback->Call(context->Global(), 0, argv);
  }
}

void drawCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::EscapableHandleScope handleScope(isolate);
  
  if(!args[0]->IsFunction()){
    // TODO: throw js exception
    //AppConsole::log("draw callback expects one argument of type function.");
    return;
  }
  //AppConsole::log("draw callback set.");
  sDrawCallback = args[0].As<v8::Function>();
  
  return;
}


int main(int argc, const char * argv[]) {
  // Initialize V8 (implicit initialization was removed in an earlier revision)
  v8::V8::Initialize();
  
  // Create a new Isolate and make it the current one.
  isolate = v8::Isolate::New();
  v8::Isolate::Scope isolate_scope(isolate);
  
  // Create a stack-allocated handle scope.
  v8::HandleScope handle_scope(isolate);
  
  // Set general globals for JS
  v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
  global->Set(v8::String::NewFromUtf8(isolate, "draw"), v8::FunctionTemplate::New(isolate, drawCallback));
  
  v8::Local<v8::Context> ct = v8::Context::New(isolate, NULL, global);
  
  
  //v8::Local<v8::Context>::New(<#v8::Isolate *isolate#>, <#const PersistentBase<v8::Context> &that#>)
}






