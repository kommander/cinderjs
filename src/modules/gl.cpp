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

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/gl/GlslProg.h"

#include "gl.hpp"

using namespace std;
using namespace cinder;
using namespace v8;

namespace cjs {
  
void GLModule::drawLine(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::EscapableHandleScope handleScope(isolate);
  
  Local<Object> vec1 = args[0].As<Object>();
  Local<Object> vec2 = args[1].As<Object>();
  
  double v1x = vec1->Get(String::NewFromUtf8(isolate, "x"))->NumberValue();
  double v1y = vec1->Get(String::NewFromUtf8(isolate, "y"))->NumberValue();
  double v2x = vec2->Get(String::NewFromUtf8(isolate, "x"))->NumberValue();
  double v2y = vec2->Get(String::NewFromUtf8(isolate, "y"))->NumberValue();
  
  gl::drawLine(Vec2f(v1x, v1y), Vec2f(v2x, v2y));
  
  return;
}

void GLModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  global->Set(v8::String::NewFromUtf8(getIsolate(), "glDrawLine"), v8::FunctionTemplate::New(getIsolate(), drawLine));
}
  
} // namespace cjs