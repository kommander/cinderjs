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

// Uncomment for debug output
//#define DEBUG_RAY_MODULE

#include "ray.hpp"
#include "AppConsole.h"
#include "../StaticFactory.hpp"

using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace v8;

namespace cjs {

vec3 RayModule::sBufVec3f_1;
vec3 RayModule::sBufVec3f_2;
vec3 RayModule::sBufVec3f_3;
vec3 RayModule::sBufVec3f_4;

void RayModule::create(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  StaticFactory::create<Ray>( isolate, args[0]->ToObject() );
  
  return;
}

void RayModule::destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<Ray>(isolate, id);
  }
  
  return;
}

void RayModule::setOrigin(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<Ray> ray = StaticFactory::get<Ray>(id);
    
    if(!ray){
      return;
    }
    
    sBufVec3f_1.x = args[1]->ToNumber()->Value();
    sBufVec3f_1.y = args[2]->ToNumber()->Value();
    sBufVec3f_1.z = args[3]->ToNumber()->Value();
    
    ray->setOrigin(sBufVec3f_1);
  }
  
  return;
}

void RayModule::getOrigin(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<Ray> ray = StaticFactory::get<Ray>(id);
    
    if(!ray){
      return;
    }
    
    sBufVec3f_1 = ray->getOrigin();
    
    Local<Array> arr = v8::Array::New(isolate);
    
    arr->Set(0, v8::Number::New(isolate, sBufVec3f_1.x));
    arr->Set(1, v8::Number::New(isolate, sBufVec3f_1.x));
    arr->Set(2, v8::Number::New(isolate, sBufVec3f_1.x));
    
    args.GetReturnValue().Set(arr);
  }
  
  return;
}

void RayModule::setDirection(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<Ray> ray = StaticFactory::get<Ray>(id);
    
    if(!ray){
      return;
    }
    
    sBufVec3f_1.x = args[1]->ToNumber()->Value();
    sBufVec3f_1.y = args[2]->ToNumber()->Value();
    sBufVec3f_1.z = args[3]->ToNumber()->Value();
    
    ray->setDirection(sBufVec3f_1);
  }
  
  return;
}

void RayModule::getDirection(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<Ray> ray = StaticFactory::get<Ray>(id);
    
    if(!ray){
      return;
    }
    
    sBufVec3f_1 = ray->getDirection();
    
    Local<Array> arr = v8::Array::New(isolate);
    
    arr->Set(0, v8::Number::New(isolate, sBufVec3f_1.x));
    arr->Set(1, v8::Number::New(isolate, sBufVec3f_1.x));
    arr->Set(2, v8::Number::New(isolate, sBufVec3f_1.x));
    
    args.GetReturnValue().Set(arr);
  }
  
  return;
}

void RayModule::calcPosition(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    double t = args[1]->ToNumber()->Value();
    
    std::shared_ptr<Ray> ray = StaticFactory::get<Ray>(id);
    
    if(!ray){
      return;
    }
    
    sBufVec3f_1 = ray->calcPosition(t);
    
    Local<Array> arr = v8::Array::New(isolate);
    
    arr->Set(0, v8::Number::New(isolate, sBufVec3f_1.x));
    arr->Set(1, v8::Number::New(isolate, sBufVec3f_1.x));
    arr->Set(2, v8::Number::New(isolate, sBufVec3f_1.x));
    
    args.GetReturnValue().Set(arr);
  }
  
  return;
}

void RayModule::calcTriangleIntersection(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<Ray> ray = StaticFactory::get<Ray>(id);
    
    if(!ray){
      return;
    }
    
    #ifdef DEBUG_RAY_MODULE
    std::cout << "calcTriangleIntersection " << to_string(id) << std::endl;
    #endif
    
    sBufVec3f_1.x = args[1]->ToNumber()->Value();
    sBufVec3f_1.y = args[2]->ToNumber()->Value();
    sBufVec3f_1.z = args[3]->ToNumber()->Value();
    
    sBufVec3f_2.x = args[4]->ToNumber()->Value();
    sBufVec3f_2.y = args[5]->ToNumber()->Value();
    sBufVec3f_2.z = args[6]->ToNumber()->Value();
    
    sBufVec3f_3.x = args[7]->ToNumber()->Value();
    sBufVec3f_3.y = args[8]->ToNumber()->Value();
    sBufVec3f_3.z = args[9]->ToNumber()->Value();
    
    float result;
    
    bool doesIntersect = ray->calcTriangleIntersection(
      sBufVec3f_1, sBufVec3f_2, sBufVec3f_3, &result
    );
    
    if(doesIntersect){
      args.GetReturnValue().Set(v8::Number::New(isolate, result));
    }
  }
  
  return;
}


// TODO:
//const Vec3f&	getInverseDirection() const { return mInvDirection; }
//char	getSignX() const { return mSignX; }
//char	getSignY() const { return mSignY; }
//char	getSignZ() const { return mSignZ; }		
//bool calcPlaneIntersection( const Vec3f &origin, const Vec3f &normal, float *result ) const;


/**
 * Add JS bindings
 */
void RayModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global ray object
  Handle<ObjectTemplate> rayTemplate = ObjectTemplate::New(getIsolate());
  
  rayTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
  rayTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroy"), v8::FunctionTemplate::New(getIsolate(), destroy));
  
  rayTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setOrigin"), v8::FunctionTemplate::New(getIsolate(), setOrigin));
  rayTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "getOrigin"), v8::FunctionTemplate::New(getIsolate(), getOrigin));
  rayTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setDirection"), v8::FunctionTemplate::New(getIsolate(), setDirection));
  rayTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "getDirection"), v8::FunctionTemplate::New(getIsolate(), getDirection));
  rayTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "calcPosition"), v8::FunctionTemplate::New(getIsolate(), calcPosition));
  rayTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "calcTriangleIntersection"), v8::FunctionTemplate::New(getIsolate(), calcTriangleIntersection));
  
  // Expose global ray object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "ray"), rayTemplate);
}
 
} // namespace cjs