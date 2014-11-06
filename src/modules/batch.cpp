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
  
  BatchRef batch;
  
  if (args.Length() > 2) {
    
    // TODO:
    //uint32_t meshOrGeomId = args[1]->ToUint32()->Value();
    
    uint32_t shaderId = args[2]->ToUint32()->Value();
    
    GlslProgRef shader = StaticFactory::get<GlslProg>(shaderId);
    
    if(!shader){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Shader (GlslProg) does not exist.")));
      return;
    }
    
    batch = Batch::create(geom::Cube(), shader);
    
    
  } else {
    return;
  }
  
  StaticFactory::put<Batch>( isolate, batch, args[0]->ToObject() );
  
  // TODO: throw isolate error on wrong arguments
  return;
}

void BatchModule::destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<Batch>(isolate, id);
  }
  
  return;
}

void BatchModule::draw(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    BatchRef batch = StaticFactory::get<Batch>(id);
    
    if(!batch){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Batch does not exist")));
      return;
    }
    
    batch->draw();
  }
  
  return;
}

//
// VertBatch

void BatchModule::createVert(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  VertBatchRef batch = VertBatch::create();
  StaticFactory::put<VertBatch>( isolate, batch, args[0]->ToObject() );
  
  return;
}

void BatchModule::destroyVert(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<VertBatch>(isolate, id);
  }
  
  return;
}

void BatchModule::drawVert(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    VertBatchRef batch = StaticFactory::get<VertBatch>(id);
    
    if(!batch){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "VertBatch does not exist")));
      return;
    }
    
    batch->draw();
  }
  
  return;
}

void BatchModule::colorVert(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    VertBatchRef batch = StaticFactory::get<VertBatch>(id);
    
    if(!batch){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "VertBatch does not exist")));
      return;
    }
    
    batch->color(
      args[1]->ToNumber()->Value(),
      args[2]->ToNumber()->Value(),
      args[3]->ToNumber()->Value(),
      args[4]->ToNumber()->Value()
    );
  }
  
  return;
}

void BatchModule::vertexVert(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    VertBatchRef batch = StaticFactory::get<VertBatch>(id);
    
    if(!batch){
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "VertBatch does not exist")));
      return;
    }
    
    if(args.Length() > 3) {
      batch->vertex(
        args[1]->ToNumber()->Value(),
        args[2]->ToNumber()->Value(),
        args[3]->ToNumber()->Value()
      );
    } else {
      batch->vertex(
        args[1]->ToNumber()->Value(),
        args[2]->ToNumber()->Value()
      );
    }
  }
  
  return;
}


// TODO:
//void	setType( GLenum type );
//GLenum	getType() const { return mPrimType; }
//
//void	normal( float x, float y, float z ) { normal( vec3( x, y, z ) ); }
//void	normal( const vec3 &n );
//
//void	texCoord( float s, float t, float r = 0, float q = 1 ) { texCoord( vec4( s, t, r, q ) ); }
//void	texCoord( const vec2 &t ) { texCoord( vec4( t.x, t.y, 0, 1 ) ); }
//void	texCoord( const vec3 &t ) { texCoord( vec4( t.x, t.y, t.z, 1 ) ); }
//void	texCoord( const vec4 &t );
//
//void	begin( GLenum type );
//void	end();
//void	clear();
//
//bool	empty() const { return mVertices.empty(); }


/**
 * Add JS bindings
 */
void BatchModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global batch object
  Handle<ObjectTemplate> batchTemplate = ObjectTemplate::New(getIsolate());
  
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroy"), v8::FunctionTemplate::New(getIsolate(), destroy));
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "draw"), v8::FunctionTemplate::New(getIsolate(), draw));
  
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "createVert"), v8::FunctionTemplate::New(getIsolate(), createVert));
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroyVert"), v8::FunctionTemplate::New(getIsolate(), destroyVert));
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawVert"), v8::FunctionTemplate::New(getIsolate(), drawVert));
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "colorVert"), v8::FunctionTemplate::New(getIsolate(), colorVert));
  batchTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "vertexVert"), v8::FunctionTemplate::New(getIsolate(), vertexVert));
  
  // Expose global batch object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "batch"), batchTemplate);
}
 
} // namespace cjs