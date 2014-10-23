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
#ifndef _CinderjsApp_hpp_
#define _CinderjsApp_hpp_

#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Filesystem.h"
#include "cinder/Timer.h"
#include "cinder/ConcurrentCircularBuffer.h"

#include <map>

#include "js_natives.h"
#include "AppConsole.h"
#include "CinderAppBase.hpp"

#include "v8.h"
#include "libplatform/libplatform.h"

// Trying not to use namespaces in header files
// using namespace cinder;

namespace cjs {

typedef boost::filesystem::path Path;

class BufferedEventHolder {
  public:
  int type;
  cinder::app::MouseEvent mEvt;
  cinder::app::KeyEvent kEvt;
  v8::Persistent<v8::Function> v8Fn;
};
typedef boost::shared_ptr<BufferedEventHolder> BufferedEvent;

class NextFrameFnHolder {
  public:
  v8::Persistent<v8::Function> v8Fn;
};
typedef boost::shared_ptr<NextFrameFnHolder> NextFrameFn;

class TimerFnHolder {
  public:
  uint32_t id;
  double after;
  double scheduledAt;
  v8::Persistent<v8::Function> v8Fn;
  bool _repeat = false;
  bool _remove = false;
};
typedef boost::shared_ptr<TimerFnHolder> TimerFn;

enum EventType {
  CJS_SHUTDOWN_REQUEST = 0,
  CJS_NEXT_FRAME = 1,
  CJS_RESIZE = 10,
  CJS_KEY_DOWN = 20,
  CJS_KEY_UP = 30
};

// TODO
// - Fix "exports" object usage in js modules (cannot be replaced with something like exports = function(){},
//   will return an empty exports object (not sure why)
// - Load a js module by dropping it on the app window
// - Split cinderjsApp in header file
// - Expose Env info like OS etc.
// - Expose versions object (cinder, v8, cinderjs)
// - uncaughtException handler

// Design Notes:
// - The implementation and native/js communication is trying to avoid object instantiation
//   to have less conversion costs when calling C++ methods from js.
//   This behaviour can be encapsuled within js itself. Converting numbers is way faster then
//   unpacking full fledged js objects to C++. ( eg. args[0]->ToObject()->Get(...) )

class CinderjsApp : public cinder::app::AppNative, public CinderAppBase  {
  public:
  CinderjsApp() : mEventQueue(1024) {}
  ~CinderjsApp(){}
  
  // Cinder App
	void setup();
  void shutdown();
  void mouseDown( cinder::app::MouseEvent event );
	void mouseMove( cinder::app::MouseEvent event );
  void keyDown( cinder::app::KeyEvent event );
  void keyUp( cinder::app::KeyEvent event );
	void update();
	void draw();
  void resize();
  void fileDrop( cinder::app::FileDropEvent event );
  
  // Threads
  void v8Thread( std::string jsFileContents );
  void v8RenderThread();
  void v8EventThread();
  void v8TimerThread( v8::Isolate* isolate );
  
  // V8 Setup
  static v8::Local<v8::Value> executeScriptString( std::string scriptStr, v8::Isolate* isolate,
    v8::Local<v8::Context> context, v8::Handle<v8::String> filename );
  
  //
  private:
  
  // v8 thread methods
  void v8Draw( double timePassed );

  
  // Stats
  volatile int v8Frames = 0;
  volatile double v8FPS = 0;
  volatile int mLastUpdate = 0;
  volatile int mUpdateInterval = 1000;
  
  // Threading
  volatile bool mShouldQuit;
  std::mutex mMainMutex;
  std::condition_variable cvJSThread;
  volatile bool _v8Run = false;
  std::condition_variable cvMainThread;
  volatile bool _mainRun = false;
  std::condition_variable cvEventThread;
  volatile bool _eventRun = false;
  
  static std::condition_variable cvTimerThread;
  static volatile bool _timerRun;
  
  cinder::app::RendererRef glRenderer;
  
  // Eventing
  cinder::ConcurrentCircularBuffer<BufferedEvent> mEventQueue;
  static cinder::ConcurrentCircularBuffer<NextFrameFn> sExecutionQueue;
  volatile cinder::Vec2f mousePosBuf;
  
  // Path
  Path mCwd;
  
  // V8 Threads
  std::shared_ptr<std::thread> mV8Thread;
  std::shared_ptr<std::thread> mV8RenderThread;
  std::shared_ptr<std::thread> mV8EventThread;
  std::shared_ptr<std::thread> mV8TimerThread;
  
  // Modules
  static v8::Persistent<v8::Object> sModuleCache;
  static v8::Persistent<v8::Array> sModuleList;
  
  // GC
  static void gcPrologueCb(v8::Isolate *isolate, v8::GCType type, v8::GCCallbackFlags flags);
  static int sGCRuns;
  
  // Default Bindings
  static void setDrawCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void setEventCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toggleAppConsole(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toggleV8Stats(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void requestQuit(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void nextFrameJS(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void setTimer(const v8::FunctionCallbackInfo<v8::Value>& args);
  
  // Default Process Bindings
  static void NativeBinding(const v8::FunctionCallbackInfo<v8::Value>& args);

  // Default Callbacks
  static v8::Persistent<v8::Function> sDrawCallback;
  static v8::Persistent<v8::Function> sEventCallback; // TODO: only push events that were subscribed to in v8
  
  //
  static v8::Persistent<v8::Object> sEmptyObject;
  
  // Console
  static volatile bool sConsoleActive;
  static volatile bool sV8StatsActive;
  
  // Error Handling
  static void handleV8TryCatch( v8::TryCatch &tryCatch );
  
  // Timers
  static cinder::ConcurrentCircularBuffer<TimerFn> sTimerQueue;
  static void executeTimer( TimerFn timer, v8::Isolate* isolate );
  static cinder::Timer sScheduleTimer;
  void v8TimerWaitingThread( double _timerWaitingFor );
  static std::condition_variable cvTimerWaitingThread;
  static volatile bool _timerWaitingRun;

  // Quit
  static bool sQuitRequested;
};

  
} // namespace cjs

#endif