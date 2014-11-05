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

#include "camera.hpp"
#include "AppConsole.h"
#include "../StaticFactory.hpp"
#include "cinder/Ray.h"

using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace v8;

namespace cjs {

vec3 CameraModule::sBufVec3f_1;
vec3 CameraModule::sBufVec3f_2;
quat CameraModule::sBufQuatf_1;

void CameraModule::create(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  StaticFactory::create<CameraPersp>( isolate, args[0]->ToObject() );
  
  return;
}

void CameraModule::destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    StaticFactory::remove<CameraPersp>(isolate, id);
  }
  
  return;
}


void CameraModule::setEyePoint(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<CameraPersp> cam = StaticFactory::get<CameraPersp>(id);
    
    if(!cam){
      return;
    }
    
    sBufVec3f_1.x = args[1]->ToNumber()->Value();
    sBufVec3f_1.y = args[2]->ToNumber()->Value();
    sBufVec3f_1.z = args[3]->ToNumber()->Value();
    
    cam->setEyePoint(sBufVec3f_1);
  }
  
  return;
}

void CameraModule::lookAt(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<CameraPersp> cam = StaticFactory::get<CameraPersp>(id);
    
    if(!cam){
      return;
    }
    
    sBufVec3f_1.x = args[1]->ToNumber()->Value();
    sBufVec3f_1.y = args[2]->ToNumber()->Value();
    sBufVec3f_1.z = args[3]->ToNumber()->Value();
    
    cam->lookAt(sBufVec3f_1);
  }
  
  return;
}

void CameraModule::setViewDirection(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<CameraPersp> cam = StaticFactory::get<CameraPersp>(id);
    
    if(!cam){
      return;
    }
    
    sBufVec3f_1.x = args[1]->ToNumber()->Value();
    sBufVec3f_1.y = args[2]->ToNumber()->Value();
    sBufVec3f_1.z = args[3]->ToNumber()->Value();
    
    cam->setViewDirection(sBufVec3f_1);
  }
  
  return;
}

void CameraModule::setOrientation(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<CameraPersp> cam = StaticFactory::get<CameraPersp>(id);
    
    if(!cam){
      return;
    }
    
    sBufQuatf_1.x = args[1]->ToNumber()->Value();
    sBufQuatf_1.y = args[2]->ToNumber()->Value();
    sBufQuatf_1.z = args[3]->ToNumber()->Value();
    
    cam->setOrientation(sBufQuatf_1);
  }
  
  return;
}

void CameraModule::generateRay(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<CameraPersp> cam = StaticFactory::get<CameraPersp>(id);
    
    if(!cam){
      return;
    }
    
    // Args u, v, aspect ration
    Ray ray = cam->generateRay(args[1]->ToNumber()->Value(), args[2]->ToNumber()->Value(), args[3]->ToNumber()->Value());
    
    StaticFactory::put<Ray>(isolate, std::shared_ptr<Ray>(new Ray(ray)), args[4]->ToObject());
  }
  
  return;
}

void CameraModule::setCenterOfInterestPoint(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<CameraPersp> cam = StaticFactory::get<CameraPersp>(id);
    
    if(!cam){
      return;
    }
    
    sBufVec3f_1.x = args[1]->ToNumber()->Value();
    sBufVec3f_1.y = args[2]->ToNumber()->Value();
    sBufVec3f_1.z = args[3]->ToNumber()->Value();
    
    cam->setCenterOfInterestPoint(sBufVec3f_1);
  }
  
  return;
}

void CameraModule::setPerspective(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if(!args[0].IsEmpty()){
    uint32_t id = args[0]->ToUint32()->Value();
    
    std::shared_ptr<CameraPersp> cam = StaticFactory::get<CameraPersp>(id);
    
    if(!cam){
      return;
    }
    
    cam->setPerspective(
      args[1]->ToNumber()->Value(),
      args[2]->ToNumber()->Value(),
      args[3]->ToNumber()->Value(),
      args[4]->ToNumber()->Value()
    );
    
  }
  
  return;
}

//void		( const Vec3f &centerOfInterestPoint );

// TODO:
//Vec3f		getEyePoint() const { return mEyePoint; }
//float		getCenterOfInterest() const { return mCenterOfInterest; }
//void		setCenterOfInterest( float aCenterOfInterest ) { mCenterOfInterest = aCenterOfInterest; }
//Vec3f		getCenterOfInterestPoint() const { return mEyePoint + mViewDirection * mCenterOfInterest; }
//Vec3f		getWorldUp() const { return mWorldUp; }
//void		setWorldUp( const Vec3f &aWorldUp );
//void		lookAt( const Vec3f &aEyePoint, const Vec3f &target );
//void		lookAt( const Vec3f &aEyePoint, const Vec3f &target, const Vec3f &aUp );
//Vec3f		getViewDirection() const { return mViewDirection; }
//Quatf		getOrientation() const { return mOrientation; }
//float	getFov() const { return mFov; }
//void	setFov( float aFov ) { mFov = aFov;  mProjectionCached = false; }
//float	getFovHorizontal() const { return toDegrees( 2.0f * math<float>::atan( math<float>::tan( toRadians(mFov) * 0.5f ) * mAspectRatio ) ); }
//void	setFovHorizontal( float aFov ) { mFov = toDegrees( 2.0f * math<float>::atan( math<float>::tan( toRadians(aFov) * 0.5f ) / mAspectRatio ) );  mProjectionCached = false; }
//float	getAspectRatio() const { return mAspectRatio; }
//void	setAspectRatio( float aAspectRatio ) { mAspectRatio = aAspectRatio; mProjectionCached = false; }
//float	getNearClip() const { return mNearClip; }
//void	setNearClip( float aNearClip ) { mNearClip = aNearClip; mProjectionCached = false; }
//float	getFarClip() const { return mFarClip; }
//void	setFarClip( float aFarClip ) { mFarClip = aFarClip; mProjectionCached = false; }
//virtual void	getNearClipCoordinates( Vec3f *topLeft, Vec3f *topRight, Vec3f *bottomLeft, Vec3f *bottomRight ) const;
//virtual void	getFarClipCoordinates( Vec3f *topLeft, Vec3f *topRight, Vec3f *bottomLeft, Vec3f *bottomRight ) const;
////! Returns the coordinates of the camera's frustum, suitable for passing to \c glFrustum
//void	getFrustum( float *left, float *top, float *right, float *bottom, float *near, float *far ) const;
////! Returns whether the camera represents a perspective projection instead of an orthographic
//virtual bool isPersp() const = 0;
//virtual const Matrix44f&	getProjectionMatrix() const { if( ! mProjectionCached ) calcProjection(); return mProjectionMatrix; }
//virtual const Matrix44f&	getModelViewMatrix() const { if( ! mModelViewCached ) calcModelView(); return mModelViewMatrix; }
//virtual const Matrix44f&	getInverseModelViewMatrix() const { if( ! mInverseModelViewCached ) calcInverseModelView(); return mInverseModelViewMatrix; }
//void	getBillboardVectors( Vec3f *right, Vec3f *up ) const;

//Vec2f worldToScreen( const Vec3f &worldCoord, float screenWidth, float screenHeight ) const;
//Vec3f worldToEye( const Vec3f &worldCoord ) { return getModelViewMatrix().transformPointAffine( worldCoord ); }
//float worldToEyeDepth( const Vec3f &worldCoord ) const { return getModelViewMatrix().m[2] * worldCoord.x + getModelViewMatrix().m[6] * worldCoord.y + getModelViewMatrix().m[10] * worldCoord.z + getModelViewMatrix().m[14]; }
//Vec3f worldToNdc( const Vec3f &worldCoord ) { Vec3f eye = getModelViewMatrix().transformPointAffine( worldCoord ); return getProjectionMatrix().transformPoint( eye ); }

//float	getScreenRadius( const class Sphere &sphere, float screenWidth, float screenHeight ) const;

/**
 * Add JS bindings
 */
void CameraModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global camera object
  Handle<ObjectTemplate> cameraTemplate = ObjectTemplate::New(getIsolate());
  
  cameraTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "create"), v8::FunctionTemplate::New(getIsolate(), create));
  cameraTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "destroy"), v8::FunctionTemplate::New(getIsolate(), destroy));
  
  cameraTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setPerspective"), v8::FunctionTemplate::New(getIsolate(), setPerspective));
  cameraTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setEyePoint"), v8::FunctionTemplate::New(getIsolate(), setEyePoint));
  cameraTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "lookAt"), v8::FunctionTemplate::New(getIsolate(), lookAt));
  cameraTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setViewDirection"), v8::FunctionTemplate::New(getIsolate(), setViewDirection));
  cameraTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setOrientation"), v8::FunctionTemplate::New(getIsolate(), setOrientation));
  cameraTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setCenterOfInterestPoint"), v8::FunctionTemplate::New(getIsolate(), setCenterOfInterestPoint));
  cameraTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "generateRay"), v8::FunctionTemplate::New(getIsolate(), generateRay));
  
  // Expose global camera object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "camera"), cameraTemplate);
}
 
} // namespace cjs