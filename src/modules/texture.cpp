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

#include "texture.hpp"
#include "AppConsole.h"
#include "../StaticFactory.hpp"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"

using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace v8;

namespace cjs {

void TextureModule::create(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  if (args.Length() > 1) {
    Local<String> lVert = args[1]->ToString();
    v8::String::Utf8Value utf8Vert(lVert);
    
    
    gl::TextureRef tex;
    
    try {
      // TODO: Take individual format when implemented..
      tex = gl::Texture::create(loadImage( getApp()->loadAsset(fs::path(*utf8Vert)) ), Texture::Format().mipmap());
    } catch(cinder::Exception &ex){
      isolate->ThrowException(v8::Exception::Error(
        v8::String::NewFromUtf8(isolate, ex.what())
      ));
      return;
    }
    
    StaticFactory::put<Texture>( isolate, tex, args[0]->ToObject() );
  }
  
  // TODO: throw isolate error on wrong arguments
  return;
}

void TextureModule::destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<Texture>(isolate, id);
  }
  
  return;
}

void TextureModule::bind(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    TextureRef tex = StaticFactory::get<Texture>(id);
    tex->bind();
  }
  
  return;
}


/**
 * Add JS bindings
 */
void TextureModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global texture object
  Handle<ObjectTemplate> textureTemplate = ObjectTemplate::New(getIsolate());
  
  textureTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
  textureTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroy"), v8::FunctionTemplate::New(getIsolate(), destroy));
  textureTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "bind"), v8::FunctionTemplate::New(getIsolate(), bind));
  
  
  // Expose global texture object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "texture"), textureTemplate);
}
 
} // namespace cjs