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

#include "shader.hpp"
#include "AppConsole.h"
#include "../StaticFactory.hpp"
#include "cinder/gl/Shader.h"

using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace v8;

namespace cjs {

void ShaderModule::create(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if (args.Length() > 1) {
    Local<String> lVert = args[1]->ToString();
    v8::String::Utf8Value utf8Vert(lVert);
    
    Local<String> lFrag = args[2]->ToString();
    v8::String::Utf8Value utf8Frag(lFrag);
    
    Local<String> lGeom = args[3]->ToString();
    v8::String::Utf8Value utf8Geom(lGeom);
    
    gl::GlslProgRef glsl;
    
    try {
      // TODO: Take prog strings optionally (-> shader.js)
      //glsl = gl::GlslProg::create( *utf8Vert, *utf8Frag );
      if( args.Length() == 2 ){
        glsl = gl::GlslProg::create(
          getApp()->loadAsset(fs::path(*utf8Vert))
        );
      }
      else if( args.Length() == 3 ){
        glsl = gl::GlslProg::create(
          getApp()->loadAsset(fs::path(*utf8Vert)),
          getApp()->loadAsset(fs::path(*utf8Frag))
        );
      }
      else if( args.Length() == 4 ){
        glsl = gl::GlslProg::create(
          getApp()->loadAsset(fs::path(*utf8Vert)),
          getApp()->loadAsset(fs::path(*utf8Frag)),
          getApp()->loadAsset(fs::path(*utf8Geom))
        );
      }
    } catch(cinder::gl::GlslProgCompileExc &ex){
      isolate->ThrowException(v8::Exception::SyntaxError(
        v8::String::NewFromUtf8(isolate, ex.what())
      ));
      return;
    } catch(cinder::Exception &ex){
      isolate->ThrowException(v8::Exception::Error(
        v8::String::NewFromUtf8(isolate, ex.what())
      ));
      return;
    }
    
    StaticFactory::put<GlslProg>( isolate, glsl, args[0]->ToObject() );
  }
  
  return;
}

void ShaderModule::createFromFormat(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if (args.Length() > 1) {
    
    uint32_t id = args[1]->ToUint32()->Value();
    
    std::shared_ptr<GlslProg::Format> format = StaticFactory::get<GlslProg::Format>(id);
    
    if(!format){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Format does not exist")));
      return;
    }
    
    gl::GlslProgRef glsl = gl::GlslProg::create( *format );
    
    StaticFactory::put<GlslProg>( isolate, glsl, args[0]->ToObject() );
  }
  
  return;
}


void ShaderModule::destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<GlslProg>(isolate, id);
  }
  
  return;
}

void ShaderModule::bind(const v8::FunctionCallbackInfo<v8::Value>& args) {
  
  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    GlslProgRef shader = StaticFactory::get<GlslProg>(id);
    
    if(!shader){
      v8::Isolate* isolate = args.GetIsolate();
      v8::HandleScope scope(isolate);
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Shader does not exist")));
      return;
    }
    
    shader->bind();
  }
  
  return;
}


void ShaderModule::uniformInt(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    GlslProgRef shader = StaticFactory::get<GlslProg>(id);
    
    if(!shader){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Shader does not exist")));
      return;
    }
    
    Local<String> locationStr = args[1]->ToString();
    v8::String::Utf8Value location(locationStr);
    const std::string loc(*location);
    
    int value = args[2]->ToUint32()->Value();
    
    shader->uniform(loc, value);
  }
  
  return;
}

void ShaderModule::uniformFloat(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    GlslProgRef shader = StaticFactory::get<GlslProg>(id);
    
    if(!shader){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Shader does not exist")));
      return;
    }
    
    Local<String> locationStr = args[1]->ToString();
    v8::String::Utf8Value location(locationStr);
    const std::string loc(*location);
    
    float value = args[2]->ToNumber()->Value();
    
    shader->uniform(loc, value);
  }
  
  return;
}

void ShaderModule::uniformVec3(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    GlslProgRef shader = StaticFactory::get<GlslProg>(id);
    
    if(!shader){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Shader does not exist")));
      return;
    }
    
    Local<String> locationStr = args[1]->ToString();
    v8::String::Utf8Value location(locationStr);
    const std::string loc(*location);
    
    uint32_t vectorId = args[2]->ToUint32()->Value();
    std::shared_ptr<vec3> vector = StaticFactory::get<vec3>(vectorId);
    
    if(!vector){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Vector does not exist")));
      return;
    }
    
    shader->uniform(loc, *vector);
  }
  
  return;
}


//
// Format
void ShaderModule::createFormat(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if (args.Length() > 0) {
    std::shared_ptr<GlslProg::Format> format(new GlslProg::Format() );
    StaticFactory::put<GlslProg::Format>( isolate, format, args[0]->ToObject() );
  }
  
  return;
}

void ShaderModule::destroyFormat(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<GlslProg::Format>(isolate, id);
  }
  
  return;
}

void ShaderModule::formatVertex(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(args[0]->IsUint32()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<GlslProg::Format> format = StaticFactory::get<GlslProg::Format>(id);
    
    if(!format){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Format does not exist")));
      return;
    }
    
    Local<String> lShader = args[1]->ToString();
    v8::String::Utf8Value utf8Shader(lShader);
    
    // TODO: catch glsl prog exceptions
    format->vertex( getApp()->loadAsset(*utf8Shader) );
  }
  
  return;
}

void ShaderModule::formatFragment(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(args[0]->IsUint32()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<GlslProg::Format> format = StaticFactory::get<GlslProg::Format>(id);
    
    if(!format){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Format does not exist")));
      return;
    }
    
    Local<String> lShader = args[1]->ToString();
    v8::String::Utf8Value utf8Shader(lShader);
    
    // TODO: catch glsl prog exceptions
    format->fragment( getApp()->loadAsset(*utf8Shader) );
  }
  
  return;
}

void ShaderModule::formatGeometry(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(args[0]->IsUint32()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<GlslProg::Format> format = StaticFactory::get<GlslProg::Format>(id);
    
    if(!format){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Format does not exist")));
      return;
    }
    
    Local<String> lShader = args[1]->ToString();
    v8::String::Utf8Value utf8Shader(lShader);
    
    // TODO: catch glsl prog exceptions
    format->geometry( getApp()->loadAsset(*utf8Shader) );
  }
  
  return;
}

void ShaderModule::formatFeedbackFormat(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(args[0]->IsUint32()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<GlslProg::Format> format = StaticFactory::get<GlslProg::Format>(id);
    
    if(!format){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Format does not exist")));
      return;
    }
    
    format->feedbackFormat( args[1]->ToUint32()->Value() );
  }
  
  return;
}

void ShaderModule::formatFeedbackVaryings(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(args[0]->IsUint32()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<GlslProg::Format> format = StaticFactory::get<GlslProg::Format>(id);
    
    if(!format){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Format does not exist")));
      return;
    }
    
    Local<Array> strArr = args[1].As<Array>();
    std::vector<std::string> strVec(strArr->Length());
    for(int i = 0; i < strArr->Length(); ++i){
      v8::String::Utf8Value v8Str( strArr->Get(i)->ToString() );
      strVec[i] = std::string(*v8Str);
    }
    
    format->feedbackVaryings(strVec);
  }
  
  return;
}

void ShaderModule::formatAttribLocation(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(args[0]->IsUint32()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<GlslProg::Format> format = StaticFactory::get<GlslProg::Format>(id);
    
    if(!format){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Format does not exist")));
      return;
    }
    
    v8::String::Utf8Value v8Str( args[1]->ToString() );
    
    format->attribLocation(std::string(*v8Str), args[2]->IntegerValue());
  }
  
  return;
}



void ShaderModule::getStockColor(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  gl::GlslProgRef shader = gl::getStockShader(ShaderDef().color());
  StaticFactory::put<gl::GlslProg>( isolate, shader, args[0]->ToObject());
  
  return;
}

void ShaderModule::getStockTexture(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  // TODO: optionally take texture target
  gl::GlslProgRef shader = gl::getStockShader(ShaderDef().texture());
  StaticFactory::put<gl::GlslProg>( isolate, shader, args[0]->ToObject());
  return;
}


/**
 * Add JS bindings
 */
void ShaderModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global shader object
  Handle<ObjectTemplate> shaderTemplate = ObjectTemplate::New(getIsolate());
  
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "createFromFormat"), v8::FunctionTemplate::New(getIsolate(), createFromFormat));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroy"), v8::FunctionTemplate::New(getIsolate(), destroy));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "bind"), v8::FunctionTemplate::New(getIsolate(), bind));
  
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "uniformInt"), v8::FunctionTemplate::New(getIsolate(), uniformInt));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "uniformFloat"), v8::FunctionTemplate::New(getIsolate(), uniformFloat));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "uniformVec3"), v8::FunctionTemplate::New(getIsolate(), uniformVec3));
  
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "createFormat"), v8::FunctionTemplate::New(getIsolate(), createFormat));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroyFormat"), v8::FunctionTemplate::New(getIsolate(), destroyFormat));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "formatVertex"), v8::FunctionTemplate::New(getIsolate(), formatVertex));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "formatFragment"), v8::FunctionTemplate::New(getIsolate(), formatFragment));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "formatGeometry"), v8::FunctionTemplate::New(getIsolate(), formatGeometry));
  shaderTemplate->Set(
    v8::String::NewFromUtf8(getIsolate(), "formatFeedbackFormat"),
    v8::FunctionTemplate::New(getIsolate(), formatFeedbackFormat)
  );
  shaderTemplate->Set(
    v8::String::NewFromUtf8(getIsolate(), "formatFeedbackVaryings"),
    v8::FunctionTemplate::New(getIsolate(), formatFeedbackVaryings)
  );
  shaderTemplate->Set(
    v8::String::NewFromUtf8(getIsolate(), "formatAttribLocation"),
    v8::FunctionTemplate::New(getIsolate(), formatAttribLocation)
  );
  
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "getStockColor"), v8::FunctionTemplate::New(getIsolate(), getStockColor));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "getStockTexture"), v8::FunctionTemplate::New(getIsolate(), getStockTexture));

  // Expose global shader object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "shader"), shaderTemplate);
}
 
} // namespace cjs