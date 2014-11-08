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
#ifndef _GLModule_hpp_
#define _GLModule_hpp_

#pragma once

#define GL_MOD_ID 1

#include "cinder/gl/GlslProg.h"
#include "cinder/Rect.h"
#include "../PipeModule.hpp"

using namespace cinder;

namespace cjs {
  
class GLModule : public PipeModule {
  
  public:
    GLModule(){}
    ~GLModule(){}
    
    inline int moduleId() {
      return GL_MOD_ID;
    }
  
    inline std::string getName() {
      return "gl";
    }
  
    void loadGlobalJS( v8::Local<v8::ObjectTemplate> &global );
    
    // Exposed
    static void drawLine(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void drawSolidCircle(const v8::FunctionCallbackInfo<v8::Value>& args);
  
    static void pushMatrices(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void popMatrices(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void pushViewport(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void popViewport(const v8::FunctionCallbackInfo<v8::Value>& args);
  
    static void translate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void scale(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void rotate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void begin(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void end(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void enable(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void disable(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void vertex(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void color(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void enableWireframe(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void disableWireframe(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void drawCube(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void drawColorCube(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void drawSphere(const v8::FunctionCallbackInfo<v8::Value>& args);
  
    static void setMatrices(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void setMatricesWindow(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void setModelMatrix(const v8::FunctionCallbackInfo<v8::Value>& args);
  
    static void clear(const v8::FunctionCallbackInfo<v8::Value>& args);
  
    static void drawTexture(const v8::FunctionCallbackInfo<v8::Value>& args);
  
    static void enableDepthRead(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void disableDepthRead(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void enableDepthWrite(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void disableDepthWrite(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void enableDepth(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void disableDepth(const v8::FunctionCallbackInfo<v8::Value>& args);
  
    static void enableVerticalSync(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void disableVerticalSync(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void isVerticalSyncEnabled(const v8::FunctionCallbackInfo<v8::Value>& args);
  
    static void multModelMatrix(const v8::FunctionCallbackInfo<v8::Value>& args);
  
  private:
    //
    static ColorA sBufColorA_1;
    static Color sBufColor_1;
    static vec2 bufVec2f_1;
    static vec2 bufVec2f_2;
    static vec3 bufVec3f_1;
    static vec3 bufVec3f_2;
    static quat bufQuat_1;
};
  
} // namespace cjs

#endif