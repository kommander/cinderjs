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

vec3 ShaderModule::sBufVec3f_1;

void ShaderModule::create(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if (args.Length() > 1) {
    Local<String> lVert = args[1]->ToString();
    v8::String::Utf8Value utf8Vert(lVert);
    
    Local<String> lFrag = args[2]->ToString();
    v8::String::Utf8Value utf8Frag(lFrag);
    
    gl::GlslProgRef glsl;
    
    try {
      // TODO: Take prog strings optionally (-> shader.js)
      //glsl = gl::GlslProg::create( *utf8Vert, *utf8Frag );
      glsl = gl::GlslProg::create(getApp()->loadAsset(fs::path(*utf8Vert)),
        getApp()->loadAsset(fs::path(*utf8Frag)));
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

void ShaderModule::destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<GlslProgRef>(isolate, id);
  }
  
  return;
}

/**
 * Add JS bindings
 */
void ShaderModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global shader object
  Handle<ObjectTemplate> shaderTemplate = ObjectTemplate::New(getIsolate());
  
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
  shaderTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroy"), v8::FunctionTemplate::New(getIsolate(), destroy));
  
  
  // Expose global shader object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "shader"), shaderTemplate);
}
 
} // namespace cjs