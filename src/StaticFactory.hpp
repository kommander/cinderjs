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
#ifndef _StaticFactory_hpp_
#define _StaticFactory_hpp_

#pragma once

#include <map>
#include <boost/shared_ptr.hpp>

#include "cinder/gl/Material.h"
#include "cinder/gl/Light.h"
#include "cinder/Ray.h"
#include "cinder/Camera.h"

namespace cjs {
  
  template<class T>
  struct FactoryTuple {
    uint32_t id;
    boost::shared_ptr<T> value;
  };
  
  class StaticFactory {
    public:
      //StaticFactory(){}
      //~StaticFactory(){}
    
      static void initialize();
    
      static FactoryTuple<cinder::gl::Material> createMaterial();
      static boost::shared_ptr<cinder::gl::Material> getMaterial( uint32_t id );
    
      static FactoryTuple<cinder::gl::Light> createLight( uint32_t type );
      static boost::shared_ptr<cinder::gl::Light> getLight( uint32_t id );
    
      static FactoryTuple<cinder::Ray> createRay();
      static boost::shared_ptr<cinder::Ray> getRay( uint32_t id );
      static uint32_t putRay(cinder::Ray &ray);
    
      static FactoryTuple<cinder::CameraPersp> createCamera();
      static boost::shared_ptr<cinder::CameraPersp> getCamera( uint32_t id );
    
      // TODO:
      //static bool removeMaterial( uint32_t id );
      //static bool removeLight( uint32_t id );
      //static bool removeRay( uint32_t id );
      // ... others
    
      // TODO:
      // To store objects that are created elsewhere
      // and have to be referenced by JS
      //static uint32_t putMaterial(boost::shared_ptr<Material> material);
      // ... others
    
    private:
      // TODO: Improve counters and reuse unused ids (generate ids at startup)
      //       -> If id space is limited, it would help to spot leaks in JS scripts
      static std::map<uint32_t, boost::shared_ptr<cinder::gl::Material>> sMaterialMap;
      static uint32_t sMaterialCounter;
      static std::map<uint32_t, boost::shared_ptr<cinder::gl::Light>> sLightMap;
      static uint32_t sLightCounter;
      static std::map<uint32_t, boost::shared_ptr<cinder::Ray>> sRayMap;
      static uint32_t sRayCounter;
      static std::map<uint32_t, boost::shared_ptr<cinder::CameraPersp>> sCameraMap;
      static uint32_t sCameraCounter;
  };
  
} // namespace

#endif