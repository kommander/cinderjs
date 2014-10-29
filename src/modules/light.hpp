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
#ifndef _LightModule_hpp_
#define _LightModule_hpp_

#pragma once

#define LIGHT_MOD_ID 6

#include "../PipeModule.hpp"
#include "cinder/gl/Light.h"

using namespace cinder;

namespace cjs {
  
class LightModule : public PipeModule {
  public:
    LightModule(){}
    ~LightModule(){}
  
    inline int moduleId() {
      return LIGHT_MOD_ID;
    }
  
    static void create(const v8::FunctionCallbackInfo<v8::Value>& args);
  
    static void enable(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void disable(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void setAmbient(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void setDiffuse(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void setSpecular(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void lookAt(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void setPosition(const v8::FunctionCallbackInfo<v8::Value>& args);
  
    void loadGlobalJS( v8::Local<v8::ObjectTemplate> &global );
    void draw(){}
  
  //
  private:
    static ColorA sBufColorA_1;
    static Vec3f sBufVec3f_1;
    static Vec3f sBufVec3f_2;

 };
  
} // namespace cjs

#endif