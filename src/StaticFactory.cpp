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

#include "StaticFactory.hpp"

using namespace cinder;
using namespace cinder::gl;
using namespace v8;
using namespace std;

namespace cjs {
  
std::map<uint32_t, boost::any> StaticFactory::_sObjectMap;

uint32_t StaticFactory::sMaterialCounter = 0;
std::map<uint32_t, boost::shared_ptr<Light>> StaticFactory::sLightMap;
uint32_t StaticFactory::sLightCounter = 0;
std::map<uint32_t, boost::shared_ptr<Ray>> StaticFactory::sRayMap;
uint32_t StaticFactory::sRayCounter = 0;
std::map<uint32_t, boost::shared_ptr<CameraPersp>> StaticFactory::sCameraMap;
uint32_t StaticFactory::sCameraCounter = 0;


//
// Light
Wrapped<Light> StaticFactory::createLight(uint32_t type){
  Wrapped<Light> tuple;
  tuple.id = sLightCounter++;
  tuple.value = boost::shared_ptr<Light>( new Light(type, tuple.id) );
  
  // Store
  sLightMap[tuple.id] = tuple.value;
  
  return tuple;
}

boost::shared_ptr<Light> StaticFactory::getLight( uint32_t id ){
  return sLightMap[id];
}

//
// Ray
Wrapped<Ray> StaticFactory::createRay(){
  Wrapped<Ray> tuple;
  tuple.id = sRayCounter++;
  tuple.value = boost::shared_ptr<Ray>( new Ray() );
  
  // Store
  sRayMap[tuple.id] = tuple.value;
  
  return tuple;
}

boost::shared_ptr<Ray> StaticFactory::getRay( uint32_t id ){
  return sRayMap[id];
}

uint32_t StaticFactory::putRay(Ray &ray){
  uint32_t id = sRayCounter++;
  boost::shared_ptr<Ray> rayPtr(new Ray(ray));
  sRayMap[id] = rayPtr;
  return id;
}

//
// Camera
Wrapped<CameraPersp> StaticFactory::createCamera(){
  Wrapped<CameraPersp> tuple;
  tuple.id = sCameraCounter++;
  
  // Use Perspective camera as default for now...
  tuple.value = boost::shared_ptr<CameraPersp>( new CameraPersp() );
  
  // Store
  sCameraMap[tuple.id] = tuple.value;
  
  return tuple;
}

boost::shared_ptr<CameraPersp> StaticFactory::getCamera( uint32_t id ){
  return sCameraMap[id];
}

  
} // namespace
