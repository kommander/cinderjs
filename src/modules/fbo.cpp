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

#include "fbo.hpp"
#include "AppConsole.h"
#include "../StaticFactory.hpp"
#include "cinder/gl/Fbo.h"

using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace v8;

namespace cjs {

void FBOModule::create(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if(args.Length() > 2){
    FboRef fbo;
    
    fbo = Fbo::create(
      args[1]->ToUint32()->Value(),  // width
      args[2]->ToUint32()->Value(),  // height
      args[3]->ToBoolean()->Value(), // alpha
      args[4]->ToBoolean()->Value(), // depth
      args[5]->ToBoolean()->Value()  // stencil
    );
    
    StaticFactory::put<Fbo>( isolate, fbo, args[0]->ToObject() );
  }
  
  
  // TODO: throw isolate error on wrong arguments
  return;
}

void FBOModule::createFromFormat(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if(args.Length() > 3){
    uint32_t id = args[3]->ToUint32()->Value();
    
    std::shared_ptr<Fbo::Format> format = StaticFactory::get<Fbo::Format>(id);
    
    if(!format){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Format does not exist")));
      return;
    }
    
    FboRef fbo = Fbo::create(
      args[1]->ToUint32()->Value(),  // width
      args[2]->ToUint32()->Value(),  // height
      *format
    );
    
    StaticFactory::put<Fbo>( isolate, fbo, args[0]->ToObject() );
  }
  // TODO: throw isolate error on wrong arguments
  return;
}

void FBOModule::destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<Fbo>(isolate, id);
  }
  
  return;
}

void FBOModule::bindBuffer(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    FboRef fbo = StaticFactory::get<Fbo>(id);
    
    if(!fbo){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Fbo does not exist")));
      return;
    }
    
    if(args.Length() == 1){
      fbo->bindFramebuffer();
    } else {
      fbo->bindFramebuffer( args[1]->ToUint32()->Value() );
    }
    
    
  }
  
  return;
}

void FBOModule::unbindBuffer(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    FboRef fbo = StaticFactory::get<Fbo>(id);
    
    if(!fbo){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Fbo does not exist")));
      return;
    }
    
    fbo->unbindFramebuffer();
  }
  
  return;
}

void FBOModule::bindTexture(const v8::FunctionCallbackInfo<v8::Value>& args) {
  
  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    FboRef fbo = StaticFactory::get<Fbo>(id);
    
    if(!fbo){
      v8::Isolate* isolate = args.GetIsolate();
      v8::HandleScope scope(isolate);
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Fbo does not exist")));
      return;
    }
    
    if(args.Length() == 1){
      fbo->bindTexture();
    } else {
      uint32_t textureUnit = 0;
      uint32_t attachment = GL_COLOR_ATTACHMENT0;
      
      if(!args[1]->IsUndefined()){
        textureUnit = args[1]->ToUint32()->Value();
      }
      
      if(!args[2]->IsUndefined()){
        attachment = args[2]->ToUint32()->Value();
      }
      
      fbo->bindTexture( textureUnit, attachment );
    }
    
    
  }
  
  return;
}

void FBOModule::unbindTexture(const v8::FunctionCallbackInfo<v8::Value>& args) {
  
  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    FboRef fbo = StaticFactory::get<Fbo>(id);
    
    if(!fbo){
      v8::Isolate* isolate = args.GetIsolate();
      v8::HandleScope scope(isolate);
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Fbo does not exist")));
      return;
    }
    
    if(args.Length() == 1){
      fbo->unbindTexture();
    } else {
      uint32_t textureUnit = 0;
      uint32_t attachment = GL_COLOR_ATTACHMENT0;
      
      if(!args[1]->IsUndefined()){
        textureUnit = args[1]->ToUint32()->Value();
      }
      
      if(!args[2]->IsUndefined()){
        attachment = args[2]->ToUint32()->Value();
      }
      
      fbo->unbindTexture( textureUnit, attachment );
    }

  }
  
  return;
}

//
// Format
void FBOModule::createFormat(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if (args.Length() > 0) {
    std::shared_ptr<Fbo::Format> format(new Fbo::Format() );
    StaticFactory::put<Fbo::Format>( isolate, format, args[0]->ToObject() );
  }
  
  // TODO: throw isolate error on wrong arguments
  return;
}

void FBOModule::destroyFormat(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<Fbo::Format>(isolate, id);
  }
  
  return;
}

void FBOModule::formatSetSamples(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(args[0]->IsUint32()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<Fbo::Format> format = StaticFactory::get<Fbo::Format>(id);
    
    if(!format){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Format does not exist")));
      return;
    }
    
    int samples = args[1]->ToUint32()->Value();
    
    format->setSamples( samples );
  }
  
  return;
}

void FBOModule::formatDepthTexture(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(args[0]->IsUint32()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<Fbo::Format> format = StaticFactory::get<Fbo::Format>(id);
    
    if(!format){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Format does not exist")));
      return;
    }
    
    // TODO: Enable texture format argument
    format->depthTexture();
  }
  
  return;
}

void FBOModule::getColorTexture(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    FboRef fbo = StaticFactory::get<Fbo>(id);
    
    if(!fbo){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Fbo does not exist")));
      return;
    }
    
    TextureRef texture = fbo->getColorTexture();
    StaticFactory::put(isolate, texture, args[1]->ToObject());
  }
  
  return;
}

void FBOModule::getDepthTexture(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    FboRef fbo = StaticFactory::get<Fbo>(id);
    
    if(!fbo){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Fbo does not exist")));
      return;
    }
    
    TextureRef texture = fbo->getDepthTexture();
    StaticFactory::put(isolate, texture, args[1]->ToObject());
  }
  
  return;
}



/**
 * Add JS bindings
 */
void FBOModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global fbo object
  Handle<ObjectTemplate> fboTemplate = ObjectTemplate::New(getIsolate());
  
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "createFromFormat"), v8::FunctionTemplate::New(getIsolate(), createFromFormat));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroy"), v8::FunctionTemplate::New(getIsolate(), destroy));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "bindBuffer"), v8::FunctionTemplate::New(getIsolate(), bindBuffer));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "unbindBuffer"), v8::FunctionTemplate::New(getIsolate(), unbindBuffer));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "bindTexture"), v8::FunctionTemplate::New(getIsolate(), bindTexture));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "unbindTexture"), v8::FunctionTemplate::New(getIsolate(), unbindTexture));
  
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "getColorTexture"), v8::FunctionTemplate::New(getIsolate(), getColorTexture));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "getDepthTexture"), v8::FunctionTemplate::New(getIsolate(), getDepthTexture));
  
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "createFormat"), v8::FunctionTemplate::New(getIsolate(), createFormat));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroyFormat"), v8::FunctionTemplate::New(getIsolate(), destroyFormat));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "formatSetSamples"), v8::FunctionTemplate::New(getIsolate(), formatSetSamples));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "formatDepthTexture"), v8::FunctionTemplate::New(getIsolate(), formatDepthTexture));
  
  
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT0"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT0));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT1"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT1));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT2"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT2));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT3"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT3));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT4"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT4));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT5"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT5));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT6"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT6));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT7"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT7));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT8"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT8));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT9"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT9));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT10"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT10));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT11"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT11));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT12"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT12));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT13"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT13));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT14"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT14));
  fboTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "GL_COLOR_ATTACHMENT15"), v8::Uint32::New(getIsolate(), GL_COLOR_ATTACHMENT15));
  
  
  // Expose global fbo object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "fbo"), fboTemplate);
}
 
} // namespace cjs