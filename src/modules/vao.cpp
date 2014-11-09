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

#include "vao.hpp"
#include "AppConsole.h"
#include "../StaticFactory.hpp"
#include "cinder/gl/Vao.h"

using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace v8;

namespace cjs {

void VAOModule::create(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  VaoRef vao = Vao::create();
  
  StaticFactory::put<Vao>( isolate, vao, args[0]->ToObject() );
  
  // TODO: throw isolate error on wrong arguments
}

void VAOModule::destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<Vao>(isolate, id);
  }
}

void VAOModule::bind(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    VaoRef vao = StaticFactory::get<Vao>(id);
    
    if(!vao){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Vao does not exist")));
      return;
    }
    
    vao->bind();
  }
}

void VAOModule::unbind(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    VaoRef vao = StaticFactory::get<Vao>(id);
    
    if(!vao){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Vao does not exist")));
      return;
    }
    
    vao->unbind();
  }
}


/**
 * Add JS bindings
 */
void VAOModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global object
  Handle<ObjectTemplate> objTemplate = ObjectTemplate::New(getIsolate());
  
  objTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
  objTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroy"), v8::FunctionTemplate::New(getIsolate(), destroy));
  objTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "bind"), v8::FunctionTemplate::New(getIsolate(), bind));
  objTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "unbind"), v8::FunctionTemplate::New(getIsolate(), unbind));
  
  
  // Expose global object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "vao"), objTemplate);
}
 
} // namespace cjs