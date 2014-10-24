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

Vec2f GLModule::bufVec2f_1;
Vec2f GLModule::bufVec2f_2;
Vec3f GLModule::bufVec3f_1;
Vec3f GLModule::bufVec3f_2;

/**
 * drawLine( sx, sy, ex, ey );
 */
void GLModule::drawLine(const v8::FunctionCallbackInfo<v8::Value>& args) {
  
  bufVec2f_1.x = args[0]->NumberValue();
  bufVec2f_1.y = args[1]->NumberValue();
  bufVec2f_2.x = args[2]->NumberValue();
  bufVec2f_2.y = args[3]->NumberValue();
  
  gl::drawLine(bufVec2f_1, bufVec2f_2);
  
  args[0].Clear();
  args[1].Clear();
  args[2].Clear();
  args[3].Clear();
  
  return;
}

/**
 * drawSolidCircle( cx, cy, radius );
 */
void GLModule::drawSolidCircle(const v8::FunctionCallbackInfo<v8::Value>& args) {
  
  bufVec2f_1.x = args[0]->NumberValue();
  bufVec2f_1.y = args[1]->NumberValue();
  double radius = args[2]->NumberValue();
  
  gl::drawSolidCircle(bufVec2f_1, radius);
  
  args[0].Clear();
  args[1].Clear();
  args[2].Clear();
  
  return;
}

/**
 *
 */
void GLModule::pushMatrices(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::pushMatrices();
  return;
}

/**
 *
 */
void GLModule::popMatrices(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::popMatrices();
  return;
}

/**
 *
 */
void GLModule::begin(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::begin(args[0]->NumberValue());
  return;
}

/**
 *
 */
void GLModule::end(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::end();
  return;
}

/**
 *
 */
void GLModule::enable(const v8::FunctionCallbackInfo<v8::Value>& args) {
  GLenum num = args[0]->ToUint32()->Value();
  gl::enable(num);
  return;
}

/**
 *
 */
void GLModule::disable(const v8::FunctionCallbackInfo<v8::Value>& args) {
  GLenum num = args[0]->ToUint32()->Value();
  gl::disable(num);
  return;
}

/**
 *
 */
void GLModule::translate(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args.Length() == 2){
    bufVec2f_1.x = args[0]->NumberValue();
    bufVec2f_1.y = args[1]->NumberValue();
    gl::translate(bufVec2f_1);
    return;
  }
  
  bufVec3f_1.x = args[0]->NumberValue();
  bufVec3f_1.y = args[1]->NumberValue();
  bufVec3f_1.z = args[2]->NumberValue();
  gl::translate(bufVec3f_1);
  return;
}

/**
 *
 */
void GLModule::scale(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args.Length() == 2){
    bufVec2f_1.x = args[0]->NumberValue();
    bufVec2f_1.y = args[1]->NumberValue();
    gl::scale(bufVec2f_1);
    return;
  }
  
  bufVec3f_1.x = args[0]->NumberValue();
  bufVec3f_1.y = args[1]->NumberValue();
  bufVec3f_1.z = args[2]->NumberValue();
  gl::scale(bufVec3f_1);
  return;
}

/**
 *
 */
void GLModule::rotate(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args.Length() == 3){
    bufVec3f_1.x = args[0]->NumberValue();
    bufVec3f_1.y = args[1]->NumberValue();
    bufVec3f_1.z = args[2]->NumberValue();
    gl::rotate(bufVec3f_1);
    
    return;
  }
  
  gl::rotate(args[0]->NumberValue());
  
  return;
}

/**
 *
 */
void GLModule::vertex(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args.Length() == 2){
    bufVec2f_1.x = args[0]->NumberValue();
    bufVec2f_1.y = args[1]->NumberValue();
    gl::vertex(bufVec2f_1);
    return;
  }
  
  bufVec3f_1.x = args[0]->NumberValue();
  bufVec3f_1.y = args[1]->NumberValue();
  bufVec3f_1.z = args[2]->NumberValue();
  gl::vertex(bufVec3f_1);
  return;
}

/**
 *
 */
void GLModule::color(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args.Length() == 3){
    gl::color(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
    return;
  }
  
  gl::color(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
  return;
}

/**
 *
 */
void GLModule::enableWireframe(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::enableWireframe();
  return;
}

/**
 *
 */
void GLModule::disableWireframe(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::disableWireframe();
  return;
}

/**
 *
 */
void GLModule::drawCube(const v8::FunctionCallbackInfo<v8::Value>& args) {
  bufVec3f_1.x = args[0]->NumberValue();
  bufVec3f_1.y = args[1]->NumberValue();
  bufVec3f_1.z = args[2]->NumberValue();
  
  bufVec3f_2.x = args[3]->NumberValue();
  bufVec3f_2.y = args[4]->NumberValue();
  bufVec3f_2.z = args[5]->NumberValue();
  
  gl::drawCube(bufVec3f_1, bufVec3f_2);
  
  return;
}

/**
 *
 */
void GLModule::drawTorus(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::drawTorus(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
  return;
}

/**
 *
 */
void GLModule::drawCylinder(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::drawCylinder(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue(), args[3]->NumberValue());
  return;
}

void GLModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global gl object
  Handle<ObjectTemplate> glTemplate = ObjectTemplate::New(getIsolate());
  
  // gl methods
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawLine"), v8::FunctionTemplate::New(getIsolate(), drawLine));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawSolidCircle"), v8::FunctionTemplate::New(getIsolate(), drawSolidCircle));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "pushMatrices"), v8::FunctionTemplate::New(getIsolate(), pushMatrices));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "popMatrices"), v8::FunctionTemplate::New(getIsolate(), popMatrices));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "translate"), v8::FunctionTemplate::New(getIsolate(), translate));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "scale"), v8::FunctionTemplate::New(getIsolate(), scale));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "rotate"), v8::FunctionTemplate::New(getIsolate(), rotate));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "begin"), v8::FunctionTemplate::New(getIsolate(), begin));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "end"), v8::FunctionTemplate::New(getIsolate(), end));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "enable"), v8::FunctionTemplate::New(getIsolate(), enable));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "disable"), v8::FunctionTemplate::New(getIsolate(), disable));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "vertex"), v8::FunctionTemplate::New(getIsolate(), vertex));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "color"), v8::FunctionTemplate::New(getIsolate(), color));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "enableWireframe"), v8::FunctionTemplate::New(getIsolate(), enableWireframe));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "disableWireframe"), v8::FunctionTemplate::New(getIsolate(), disableWireframe));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawCube"), v8::FunctionTemplate::New(getIsolate(), drawCube));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawTorus"), v8::FunctionTemplate::New(getIsolate(), drawTorus));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawCylinder"), v8::FunctionTemplate::New(getIsolate(), drawCylinder));
  
  // Some GL Constants
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "CULL_FACE"), v8::Uint32::New(getIsolate(), GL_CULL_FACE));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "CULL_FACE_MODE"), v8::Uint32::New(getIsolate(), GL_CULL_FACE_MODE));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHTING"), v8::Uint32::New(getIsolate(), GL_LIGHTING));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT0"), v8::Uint32::New(getIsolate(), GL_LIGHT0));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT1"), v8::Uint32::New(getIsolate(), GL_LIGHT1));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT2"), v8::Uint32::New(getIsolate(), GL_LIGHT2));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT3"), v8::Uint32::New(getIsolate(), GL_LIGHT3));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT4"), v8::Uint32::New(getIsolate(), GL_LIGHT4));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT5"), v8::Uint32::New(getIsolate(), GL_LIGHT5));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT6"), v8::Uint32::New(getIsolate(), GL_LIGHT6));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LIGHT7"), v8::Uint32::New(getIsolate(), GL_LIGHT7));
  
  // Expose global gl object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "gl"), glTemplate);
}
  
} // namespace cjs