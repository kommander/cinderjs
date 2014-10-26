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

namespace cjs {

std::map<uint32_t, boost::shared_ptr<Material>> StaticFactory::sMaterialMap;
uint32_t StaticFactory::sMaterialCounter = 0;

FactoryTuple<Material> StaticFactory::createMaterial(){
  FactoryTuple<Material> matObj;
  matObj.id = sMaterialCounter++;
  matObj.value = boost::shared_ptr<Material>( new Material() );
  
  // Store
  sMaterialMap[matObj.id] = matObj.value;
  
  return matObj;
}

boost::shared_ptr<Material> StaticFactory::getMaterial( uint32_t id ){
  return sMaterialMap[id];
}
  
  
}
