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

#include "batch.hpp"
#include "AppConsole.h"
#include "../StaticFactory.hpp"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Shader.h"

using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace v8;

namespace cjs {

vec3 BatchModule::sBufVec3f_1;

void BatchModule::create(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if (args.Length() > 2) {
    
    //uint32_t meshOrGeomId = args[1]->ToUint32()->Value();
    
    uint32_t shaderId = args[2]->ToUint32()->Value();
    
    GlslProgRef shader = StaticFactory::get<GlslProg>(shaderId);
    
    if(!shader){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Shader (GlslProg) does not exist.")));
      return;
    }
    
    BatchRef batch = Batch::create(geom::Cube(), shader);
    
    StaticFactory::put<Batch>( isolate, batch, args[0]->ToObject() );
  }
  // TODO: throw isolate error on wrong arguments
  return;
}

void BatchModule::destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<GlslProgRef>(isolate, id);
  }
  
  return;
}

void BatchModule::draw(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    BatchRef batch = StaticFactory::get<Batch>(id);
    batch->draw();
  }
  
  return;
}


/**
 * Add JS bindings
 */
void BatchModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global batch object
  Handle<ObjectTemplate> batchTemplate = ObjectTemplate::New(getIsolate());
  
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroy"), v8::FunctionTemplate::New(getIsolate(), destroy));
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "draw"), v8::FunctionTemplate::New(getIsolate(), draw));
  
  
  // Expose global batch object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "batch"), batchTemplate);
}
 
} // namespace cjs