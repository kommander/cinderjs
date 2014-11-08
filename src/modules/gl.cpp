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

#include "../StaticFactory.hpp"

#include "gl.hpp"

using namespace std;
using namespace cinder;
using namespace v8;

// NOTE: Scoped features won't work currently, using non-scoped versions

namespace cjs {

vec2 GLModule::bufVec2f_1;
vec2 GLModule::bufVec2f_2;
vec3 GLModule::bufVec3f_1;
vec3 GLModule::bufVec3f_2;
quat GLModule::bufQuat_1;
ColorA GLModule::sBufColorA_1;
Color GLModule::sBufColor_1;

/**
 * drawLine( sx, sy, ex, ey );
 */
void GLModule::drawLine(const v8::FunctionCallbackInfo<v8::Value>& args) {
  
  // 2D Line
  if(args.Length() == 4){
    bufVec2f_1.x = args[0]->NumberValue();
    bufVec2f_1.y = args[1]->NumberValue();
    bufVec2f_2.x = args[2]->NumberValue();
    bufVec2f_2.y = args[3]->NumberValue();
    
    gl::drawLine(bufVec2f_1, bufVec2f_2);
  }
  
  // 3D Line
  else {
    bufVec3f_1.x = args[0]->NumberValue();
    bufVec3f_1.y = args[1]->NumberValue();
    bufVec3f_1.z = args[2]->NumberValue();
    bufVec3f_2.x = args[3]->NumberValue();
    bufVec3f_2.y = args[4]->NumberValue();
    bufVec3f_2.z = args[5]->NumberValue();
    
    gl::drawLine(bufVec3f_1, bufVec3f_2);
  }
  
  
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
    gl::translate(vec2(args[0]->NumberValue(), args[1]->NumberValue()));
    return;
  }
  
  gl::translate(vec3(
    args[0]->NumberValue(),
    args[1]->NumberValue(),
    args[2]->NumberValue()
  ));
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
  if(args.Length() >= 3){
    
    gl::rotate(quat(
      args[0]->NumberValue(),
      args[1]->NumberValue(),
      args[2]->NumberValue(),
      args[3]->NumberValue()
    ));
    
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
void GLModule::enableDepthRead(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::enableDepthRead();
  return;
}

/**
 *
 */
void GLModule::disableDepthRead(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::disableDepthRead();
  return;
}

/**
 *
 */
void GLModule::enableDepthWrite(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::enableDepthWrite();
  return;
}

/**
 *
 */
void GLModule::disableDepthWrite(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::disableDepthWrite();
  return;
}

/**
 *
 */
void GLModule::enableDepth(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::enableDepthRead();
  gl::enableDepthWrite();
  return;
}

/**
 *
 */
void GLModule::disableDepth(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::disableDepthRead();
  gl::disableDepthWrite();
  return;
}


/**
 *
 */
void GLModule::enableVerticalSync(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::enableVerticalSync();
  return;
}

/**
 *
 */
void GLModule::disableVerticalSync(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::enableVerticalSync( false );
  return;
}

/**
 *
 */
void GLModule::isVerticalSyncEnabled(const v8::FunctionCallbackInfo<v8::Value>& args) {
  args.GetReturnValue().Set(gl::isVerticalSyncEnabled());
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
void GLModule::drawColorCube(const v8::FunctionCallbackInfo<v8::Value>& args) {
  
  bufVec3f_1.x = args[0]->NumberValue();
  bufVec3f_1.y = args[1]->NumberValue();
  bufVec3f_1.z = args[2]->NumberValue();
  
  bufVec3f_2.x = args[3]->NumberValue();
  bufVec3f_2.y = args[4]->NumberValue();
  bufVec3f_2.z = args[5]->NumberValue();
  
  gl::drawColorCube(bufVec3f_1, bufVec3f_2);
  
  return;
}

/**
 *
 */
void GLModule::drawSphere(const v8::FunctionCallbackInfo<v8::Value>& args) {
  bufVec3f_1.x = args[0]->NumberValue();
  bufVec3f_1.y = args[1]->NumberValue();
  bufVec3f_1.z = args[2]->NumberValue();
  
  // ( const Vec3f &center, float radius, int segments = 12 )
  gl::drawSphere(bufVec3f_1, args[3]->NumberValue(), args[4]->NumberValue());
  
  return;
}

/**
 *
 */
void GLModule::setMatrices(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args[0]->IsUint32()){
    uint32_t id = args[0]->ToUint32()->Value();
    std::shared_ptr<CameraPersp> cam = StaticFactory::get<CameraPersp>(id);
    
    if(!cam){
      Isolate* isolate = args.GetIsolate();
      HandleScope scope(isolate);
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Camera does not exist")));
      return;
    }
    
    gl::setMatrices(*cam);
    
    return;
  } else {
    // Error, need a camera id
    Isolate* isolate = args.GetIsolate();
    HandleScope scope(isolate);
    isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Need a camera id (uint32)")));
  }
}

/**
 *
 */
void GLModule::setMatricesWindow(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args.Length() == 2) {
    gl::setMatricesWindow(args[0]->ToUint32()->Value(), args[1]->ToUint32()->Value());
  }
  return;
}

/**
 *
 */
void GLModule::setModelMatrix(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args[0]->IsUint32()){
    uint32_t id = args[0]->ToUint32()->Value();
    std::shared_ptr<mat4> mat = StaticFactory::get<mat4>(id);
    
    if(!mat){
      Isolate* isolate = args.GetIsolate();
      HandleScope scope(isolate);
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Matrix does not exist")));
      return;
    }
    
    gl::setModelMatrix(*mat);
    
    return;
  } else {
    // Error
    Isolate* isolate = args.GetIsolate();
    HandleScope scope(isolate);
    isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Need a Matrix id (uint32)")));
  }
}


/**
 *
 */
void GLModule::clear(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args.Length() >= 3) {
    sBufColor_1.r = args[0]->ToNumber()->Value();
    sBufColor_1.g = args[1]->ToNumber()->Value();
    sBufColor_1.b = args[2]->ToNumber()->Value();
    
    return gl::clear( sBufColor_1 );
  }
  
  gl::clear();
  return;
}

/**
 *
 */
void GLModule::multModelMatrix(const v8::FunctionCallbackInfo<v8::Value>& args) {
  uint32_t id = args[0]->ToUint32()->Value();
  std::shared_ptr<mat4> matrix = StaticFactory::get<mat4>(id);
  
  if(!matrix){
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);
    isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Matrix does not exist")));
    return;
  }
  
  gl::multModelMatrix(*matrix);
  return;
}

/**
 *
 */
void GLModule::pushViewport(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args.Length() > 3) {
    return gl::pushViewport(
      vec2(
        args[0]->ToNumber()->Value(),
        args[1]->ToNumber()->Value()
      ),
      vec2(
        args[2]->ToNumber()->Value(),
        args[3]->ToNumber()->Value()
      )
    );
  }
  
  gl::pushViewport();
  return;
}

/**
 *
 */
void GLModule::popViewport(const v8::FunctionCallbackInfo<v8::Value>& args) {
  gl::popViewport();
  return;
}

/**
 *
 */
void GLModule::drawTexture(const v8::FunctionCallbackInfo<v8::Value>& args) {
  
  // gl::draw(const Texture2dRef &texture, const Rectf &dstRect)
  if(args.Length() == 5){
    uint32_t id = args[0]->ToUint32()->Value();
    
    gl::TextureRef texture = StaticFactory::get<gl::Texture>(id);
    
    if(!texture){
      v8::Isolate* isolate = args.GetIsolate();
      v8::HandleScope scope(isolate);
      isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Texture does not exist")));
      return;
    }
    
    gl::draw(texture, Rectf(
      args[1]->ToNumber()->Value(),
      args[2]->ToNumber()->Value(),
      args[3]->ToNumber()->Value(),
      args[4]->ToNumber()->Value()
    ));
  }
  
  // TODO: Implement gl::draw(const Texture2dRef &texture, const cinder::Area &srcArea, const Rectf &dstRect)
  
  return;
}


// TODO
//void setModelView( const Camera &cam );
//void setProjection( const Camera &cam );

void GLModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global gl object
  Handle<ObjectTemplate> glTemplate = ObjectTemplate::New(getIsolate());
  
  // gl methods
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setMatrices"), v8::FunctionTemplate::New(getIsolate(), setMatrices));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setMatricesWindow"), v8::FunctionTemplate::New(getIsolate(), setMatricesWindow));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setModelMatrix"), v8::FunctionTemplate::New(getIsolate(), setModelMatrix));
  
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "clear"), v8::FunctionTemplate::New(getIsolate(), clear));
  
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawTexture"), v8::FunctionTemplate::New(getIsolate(), drawTexture));
  
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawLine"), v8::FunctionTemplate::New(getIsolate(), drawLine));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawSolidCircle"), v8::FunctionTemplate::New(getIsolate(), drawSolidCircle));
  
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "pushMatrices"), v8::FunctionTemplate::New(getIsolate(), pushMatrices));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "popMatrices"), v8::FunctionTemplate::New(getIsolate(), popMatrices));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "pushViewport"), v8::FunctionTemplate::New(getIsolate(), pushViewport));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "popViewport"), v8::FunctionTemplate::New(getIsolate(), popViewport));
  
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
  
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "enableDepthRead"), v8::FunctionTemplate::New(getIsolate(), enableDepthRead));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "disableDepthRead"), v8::FunctionTemplate::New(getIsolate(), disableDepthRead));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "enableDepthWrite"), v8::FunctionTemplate::New(getIsolate(), enableDepthWrite));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "disableDepthWrite"), v8::FunctionTemplate::New(getIsolate(), disableDepthWrite));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "enableDepth"), v8::FunctionTemplate::New(getIsolate(), enableDepth));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "disableDepth"), v8::FunctionTemplate::New(getIsolate(), disableDepth));
  
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "enableVerticalSync"), v8::FunctionTemplate::New(getIsolate(), enableVerticalSync));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "disableVerticalSync"), v8::FunctionTemplate::New(getIsolate(), disableVerticalSync));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "isVerticalSyncEnabled"), v8::FunctionTemplate::New(getIsolate(), isVerticalSyncEnabled));
  
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "multModelMatrix"), v8::FunctionTemplate::New(getIsolate(), multModelMatrix));
  
  // Primitives
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawCube"), v8::FunctionTemplate::New(getIsolate(), drawCube));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawColorCube"), v8::FunctionTemplate::New(getIsolate(), drawColorCube));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawSphere"), v8::FunctionTemplate::New(getIsolate(), drawSphere));
  
  // Some GL Constants
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "CULL_FACE"), v8::Uint32::New(getIsolate(), GL_CULL_FACE));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "CULL_FACE_MODE"), v8::Uint32::New(getIsolate(), GL_CULL_FACE_MODE));
  
  
  // Primitive Types
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "POINTS"), v8::Uint32::New(getIsolate(), GL_POINTS));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "LINES"), v8::Uint32::New(getIsolate(), GL_LINES));
  glTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "TRIANGLES"), v8::Uint32::New(getIsolate(), GL_TRIANGLES));
  
  // TODO: Types
//  LINE_LOOP					= 0x0002
//	LINE_STRIP					= 0x0003
//	TRIANGLE_STRIP					= 0x0005
//	TRIANGLE_FAN					= 0x0006
//	QUADS						= 0x0007
//	QUAD_STRIP					= 0x0008
	
  // Expose global gl object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "gl"), glTemplate);
}
  
} // namespace cjs