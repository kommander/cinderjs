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
//#define DEBUG_LIGHT_MODULE

#include "light.hpp"
#include "AppConsole.h"
#include "../StaticFactory.hpp"

using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace v8;

namespace cjs {

ColorA LightModule::sBufColorA_1;
Vec3f LightModule::sBufVec3f_1;
Vec3f LightModule::sBufVec3f_2;

// TODO: handle light ids
uint16_t sLightIds = 0;

void LightModule::create(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  uint32_t type = args[1]->ToUint32()->Value();
  
  StaticFactory::put<Light>( isolate, boost::shared_ptr<Light>(new Light(type, sLightIds++)), args[0]->ToObject() );
  
  #ifdef DEBUG_LIGHT_MODULE
  std::cout << "created light " << to_string(id) << "/" << to_string(sLightObjects.size()) << std::endl;
  #endif
  
  return;
}

void LightModule::enable(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Light> light = StaticFactory::get<Light>(id);
    
    if(!light){
      return;
    }
    
    #ifdef DEBUG_LIGHT_MODULE
    std::cout << "enable light " << to_string(id) << std::endl;
    #endif
    
    light->enable();
  }
  
  return;
}

void LightModule::disable(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Light> light = StaticFactory::get<Light>(id);
    
    if(!light){
      return;
    }
    
    #ifdef DEBUG_LIGHT_MODULE
    std::cout << "disable light " << to_string(id) << std::endl;
    #endif
    
    light->disable();
  }
  
  return;
}

void LightModule::setAmbient(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Light> light = StaticFactory::get<Light>(id);
    
    if(!light){
      return;
    }
    
    sBufColorA_1.r = args[1]->ToNumber()->Value();
    sBufColorA_1.g = args[2]->ToNumber()->Value();
    sBufColorA_1.b = args[3]->ToNumber()->Value();
    sBufColorA_1.a = args[4]->ToNumber()->Value();
    
    #ifdef DEBUG_LIGHT_MODULE
    std::cout << "light setAmbient " << to_string(id) << std::endl;
    #endif
    
    light->setAmbient(sBufColorA_1);
    
  }
  
  return;
}

void LightModule::setDiffuse(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Light> light = StaticFactory::get<Light>(id);
    
    if(!light){
      return;
    }
    
    sBufColorA_1.r = args[1]->ToNumber()->Value();
    sBufColorA_1.g = args[2]->ToNumber()->Value();
    sBufColorA_1.b = args[3]->ToNumber()->Value();
    sBufColorA_1.a = args[4]->ToNumber()->Value();
    
    #ifdef DEBUG_LIGHT_MODULE
    std::cout << "light setDiffuse " << to_string(id) << std::endl;
    #endif
    
    light->setDiffuse(sBufColorA_1);
    
  }
  
  return;
}

void LightModule::setSpecular(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Light> light = StaticFactory::get<Light>(id);
    
    if(!light){
      return;
    }
    
    sBufColorA_1.r = args[1]->ToNumber()->Value();
    sBufColorA_1.g = args[2]->ToNumber()->Value();
    sBufColorA_1.b = args[3]->ToNumber()->Value();
    sBufColorA_1.a = args[4]->ToNumber()->Value();
    
    #ifdef DEBUG_LIGHT_MODULE
    std::cout << "light setSpecular " << to_string(id) << std::endl;
    #endif
    
    light->setSpecular(sBufColorA_1);
    
  }
  
  return;
}

void LightModule::lookAt(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Light> light = StaticFactory::get<Light>(id);
    
    if(!light){
      return;
    }
    
    sBufVec3f_1.x = args[1]->ToNumber()->Value();
    sBufVec3f_1.y = args[2]->ToNumber()->Value();
    sBufVec3f_1.z = args[3]->ToNumber()->Value();
    
    sBufVec3f_2.x = args[4]->ToNumber()->Value();
    sBufVec3f_2.y = args[5]->ToNumber()->Value();
    sBufVec3f_2.z = args[6]->ToNumber()->Value();
    
    #ifdef DEBUG_LIGHT_MODULE
    std::cout << "light lookAt " << to_string(id) << std::endl;
    #endif
    
    light->lookAt(sBufVec3f_1, sBufVec3f_2);
    
  }
  
  return;
}

void LightModule::setPosition(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    boost::shared_ptr<Light> light = StaticFactory::get<Light>(id);
    
    if(!light){
      return;
    }
    
    sBufVec3f_1.x = args[1]->ToNumber()->Value();
    sBufVec3f_1.y = args[2]->ToNumber()->Value();
    sBufVec3f_1.z = args[3]->ToNumber()->Value();
    
    #ifdef DEBUG_LIGHT_MODULE
    std::cout << "light setPosition " << to_string(id) << std::endl;
    #endif
    
    light->setPosition(sBufVec3f_1);
  }
  
  return;
}

// TODO:
//	void	setDirection( const Vec3f &aDirection );
//	Vec3f	getPosition() const { return mPosition; }
//
//	void setAttenuation( float aConstantAttenuation, float aLinearAttenuation = 1.0f, float aQuadraticAttenuation = 1.0f );
//	void setConstantAttenuation( float aConstantAttenuation );
//	void setLinearAttenuation( float aLinearAttenuation );
//	void setQuadraticAttenuation( float aQuadraticAttenuation );
//	
//	void setSpotExponent( float aSpotExponent );
//	void setSpotCutoff( float aSpotCutoff );
//	
//	void update( const Camera &relativeCamera ) const;
//
//	void setShadowParams( float aShadowFOV, float aShadowNear, float aShadowFar );
//	void setShadowRenderMatrices() const;
//	Matrix44f getShadowTransformationMatrix( const Camera &camera ) const;
//	const CameraPersp&	getShadowCamera() const { return mShadowCam; }
//	
//	enum Type { POINT, DIRECTIONAL, SPOTLIGHT };

/**
 * Add JS bindings
 */
void LightModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global light object
  Handle<ObjectTemplate> lightTemplate = ObjectTemplate::New(getIsolate());
  
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
  
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "enable"), v8::FunctionTemplate::New(getIsolate(), enable));
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "disable"), v8::FunctionTemplate::New(getIsolate(), disable));
  
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setAmbient"), v8::FunctionTemplate::New(getIsolate(), setAmbient));
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setDiffuse"), v8::FunctionTemplate::New(getIsolate(), setDiffuse));
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setSpecular"), v8::FunctionTemplate::New(getIsolate(), setSpecular));
  
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "lookAt"), v8::FunctionTemplate::New(getIsolate(), lookAt));
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setPosition"), v8::FunctionTemplate::New(getIsolate(), setPosition));
  
  // Types
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT_POINT"), v8::Uint32::New(getIsolate(), Light::POINT));
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT_DIRECTIONAL"), v8::Uint32::New(getIsolate(), Light::DIRECTIONAL));
  lightTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT_SPOTLIGHT"), v8::Uint32::New(getIsolate(), Light::SPOTLIGHT));
  
  // Expose global light object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "light"), lightTemplate);
}
 
} // namespace cjs