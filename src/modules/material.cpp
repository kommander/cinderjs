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

#include "material.hpp"
#include "AppConsole.h"
#include "../StaticFactory.hpp"

#include "cinder/gl/Material.h"

#include <assert.h>

using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace v8;

namespace cjs {

// TODO: Move to static module methods

ColorA MaterialModule::sBufColorA_1;

void MaterialModule::create(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  Local<Object> idHolder = StaticFactory::create<Material>( isolate );
  
  //std::cout << "created material material " << to_string(id) << "/" << to_string(sMaterialObjects.size()) << std::endl;
  
  assert(idHolder->IsObject());
  assert(idHolder->Has(v8::String::NewFromUtf8(isolate, "id")));
  
  args.GetReturnValue().Set(idHolder);
  return;
}

void MaterialModule::apply(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    //std::cout << "trying to apply material " << to_string(id) << std::endl;
    
    boost::shared_ptr<Material> material = StaticFactory::get<Material>(id);
    
    if(!material){
      return;
    }
    
    //std::cout << "apply material " << to_string(id) << std::endl;
    material->apply();
  }
  
  return;
}

void MaterialModule::setAmbient(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Material> material = StaticFactory::get<Material>(id);
    
    if(!material){
      return;
    }
    
    sBufColorA_1.r = args[1]->ToNumber()->Value();
    sBufColorA_1.g = args[2]->ToNumber()->Value();
    sBufColorA_1.b = args[3]->ToNumber()->Value();
    sBufColorA_1.a = args[4]->ToNumber()->Value();
    
    //std::cout << "set ambient " << to_string(id) << std::endl;
    material->setAmbient(sBufColorA_1);
    
  }
  
  return;
}

void MaterialModule::setDiffuse(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Material> material = StaticFactory::get<Material>(id);
    
    if(!material){
      return;
    }
    
    sBufColorA_1.r = args[1]->ToNumber()->Value();
    sBufColorA_1.g = args[2]->ToNumber()->Value();
    sBufColorA_1.b = args[3]->ToNumber()->Value();
    sBufColorA_1.a = args[4]->ToNumber()->Value();
    
    //std::cout << "set diffuse " << to_string(id) << std::endl;
    material->setDiffuse(sBufColorA_1);
    
  }
  
  return;
}

void MaterialModule::setSpecular(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Material> material = StaticFactory::get<Material>(id);
    
    if(!material){
      return;
    }
    
    sBufColorA_1.r = args[1]->ToNumber()->Value();
    sBufColorA_1.g = args[2]->ToNumber()->Value();
    sBufColorA_1.b = args[3]->ToNumber()->Value();
    sBufColorA_1.a = args[4]->ToNumber()->Value();
    
    //std::cout << "set specular " << to_string(id) << std::endl;
    material->setSpecular(sBufColorA_1);
    
  }
  
  return;
}

void MaterialModule::setEmission(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Material> material = StaticFactory::get<Material>(id);
    
    if(!material){
      return;
    }
    
    sBufColorA_1.r = args[1]->ToNumber()->Value();
    sBufColorA_1.g = args[2]->ToNumber()->Value();
    sBufColorA_1.b = args[3]->ToNumber()->Value();
    sBufColorA_1.a = args[4]->ToNumber()->Value();
    
    //std::cout << "set emission " << to_string(id) << std::endl;
    material->setEmission(sBufColorA_1);
    
  }
  
  return;
}

void MaterialModule::setShininess(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Material> material = StaticFactory::get<Material>(id);
    
    if(!material){
      return;
    }
    
    double shiny = args[1]->ToNumber()->Value();
    
    //std::cout << "set shininess " << to_string(id) << std::endl;
    material->setShininess(shiny);
    
  }
  
  return;
}

// TODO:
// void	setFace( GLenum aFace ) { mFace = aFace; }

/**
 * Add JS bindings
 */
void MaterialModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global material object
  Handle<ObjectTemplate> materialTemplate = ObjectTemplate::New(getIsolate());
  
  materialTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
 
  materialTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "apply"), v8::FunctionTemplate::New(getIsolate(), apply));
  materialTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setAmbient"), v8::FunctionTemplate::New(getIsolate(), setAmbient));
  materialTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setDiffuse"), v8::FunctionTemplate::New(getIsolate(), setDiffuse));
  materialTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setSpecular"), v8::FunctionTemplate::New(getIsolate(), setSpecular));
  materialTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setEmission"), v8::FunctionTemplate::New(getIsolate(), setEmission));
  materialTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setShininess"), v8::FunctionTemplate::New(getIsolate(), setShininess));
  
  // Expose global material object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "material"), materialTemplate);
}
 
} // namespace cjs