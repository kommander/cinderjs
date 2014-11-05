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

#include "glm.hpp"
#include "AppConsole.h"
#include "../StaticFactory.hpp"
#include "glm/glm.hpp"

using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace v8;

namespace cjs {

void GlmModule::createMat4(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if (args.Length() > 0) {
    
    if(args.Length() == 17){
      std::shared_ptr<mat4> matrix( new mat4(
        args[1]->ToNumber()->Value(), // const X1 &x1
        args[2]->ToNumber()->Value(), // const Y1 &y1
        args[3]->ToNumber()->Value(), // const Z1 &z1
        args[4]->ToNumber()->Value(), // const W1 &w1
        args[5]->ToNumber()->Value(), // const X2 &x2
        args[6]->ToNumber()->Value(), // const Y2 &y2
        args[7]->ToNumber()->Value(), // const Z2 &z2
        args[8]->ToNumber()->Value(), // const W2 &w2
        args[9]->ToNumber()->Value(), // const X3 &x3
        args[10]->ToNumber()->Value(), // const Y3 &y3
        args[11]->ToNumber()->Value(), // const Z3 &z3
        args[12]->ToNumber()->Value(), // const W3 &w3
        args[13]->ToNumber()->Value(), // const X4 &x4
        args[14]->ToNumber()->Value(), // const Y4 &y4
        args[15]->ToNumber()->Value(), // const Z4 &z4
        args[16]->ToNumber()->Value()  // const W4 &w4
      ));
      StaticFactory::put<mat4>( isolate, matrix, args[0]->ToObject() );
    } else {
      std::shared_ptr<mat4> matrix( new mat4() );
      StaticFactory::put<mat4>( isolate, matrix, args[0]->ToObject() );
    }
    
    return;
  }
  // TODO: throw isolate error on wrong arguments
}

void GlmModule::destroyMat4(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<mat4>(isolate, id);
  }
  
  return;
}

void GlmModule::rotate(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if(args.Length() > 0){
    
    float angle = args[1]->ToNumber()->Value();
    
    mat4 rotationMatrix = glm::rotate(angle, normalize( vec3(
      args[2]->ToNumber()->Value(),
      args[3]->ToNumber()->Value(),
      args[4]->ToNumber()->Value()
    )));
    
    StaticFactory::put<mat4>( isolate, std::shared_ptr<mat4>(new mat4(rotationMatrix)), args[0]->ToObject() );
  }
  
  return;
}

void GlmModule::multMat4(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if(!args[0].IsEmpty()){
    uint32_t id1 = args[0]->ToUint32()->Value();
    uint32_t id2 = args[1]->ToUint32()->Value();
    
    std::shared_ptr<mat4> matrix1 = StaticFactory::get<mat4>(id1);
    std::shared_ptr<mat4> matrix2 = StaticFactory::get<mat4>(id2);
    
    matrix1->operator*=(*matrix2);
  }
  
  return;
}


/**
 * Add JS bindings
 */
void GlmModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global glm object
  Handle<ObjectTemplate> glmTemplate = ObjectTemplate::New(getIsolate());
  
  glmTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "createMat4"), v8::FunctionTemplate::New(getIsolate(), createMat4));
  glmTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroyMat4"), v8::FunctionTemplate::New(getIsolate(), destroyMat4));
  glmTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "rotate"), v8::FunctionTemplate::New(getIsolate(), rotate));
  glmTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "multMat4"), v8::FunctionTemplate::New(getIsolate(), multMat4));
  
  
  // Expose global glm object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "glm"), glmTemplate);
}
 
} // namespace cjs