#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Filesystem.h"
#include "cinder/ConcurrentCircularBuffer.h"

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <string.h>
#include <fstream>
#include <sstream>
#include <cerrno>

#include "js_natives.h"
#include "AppConsole.h"
#include "CinderAppBase.hpp"

#include "v8.h"
#include "libplatform/libplatform.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#endif

// Modules
#include "modules/app.hpp"
#include "modules/gl.hpp"
#include "modules/console.hpp"
#include "modules/utils.hpp"
#include "modules/fs.hpp"
#include "modules/vm.hpp"


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace v8;

namespace cjs {

typedef boost::filesystem::path Path;

class BufferedEventHolder {
  public:
  int type;
  MouseEvent mEvt;
  KeyEvent kEvt;
  v8::Persistent<v8::Function> v8Fn;
};
typedef boost::shared_ptr<BufferedEventHolder> BufferedEvent;

class NextFrameFnHolder {
  public:
  v8::Persistent<v8::Function> v8Fn;
};
typedef boost::shared_ptr<NextFrameFnHolder> NextFrameFn;

enum EventType {
  CJS_SHUTDOWN_REQUEST = 0,
  CJS_NEXT_FRAME = 1,
  CJS_RESIZE = 10,
  CJS_KEY_DOWN = 20,
  CJS_KEY_UP = 30
};

// TODO
// - Load a js module by dropping it on the app window
// - Split cinderjsApp in header file
// - load js modules with wrapper: "function (module, exports, __filename, ...) {"
// - Expose Env info like OS etc.
// - Expose versions object (cinder, v8, cinderjs)

class CinderjsApp : public AppNative, public CinderAppBase  {
  public:
  CinderjsApp() : mEventQueue(1024) {}
  ~CinderjsApp(){}
  
  // Cinder App
	void setup();
  void shutdown();
	void mouseDown( MouseEvent event );	
	void mouseMove( MouseEvent event );
  void keyDown( KeyEvent event );
  void keyUp( KeyEvent event );
	void update();
	void draw();
  void resize();
  
  // Threads
  void v8Thread( std::string jsFileContents );
  void v8RenderThread();
  void v8EventThread();
  
  // V8 Setup
  static v8::Local<v8::Value> executeScriptString( std::string scriptStr, Isolate* isolate,
    Local<Context> context, Handle<String> filename );
  
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
  
  RendererRef glRenderer;
  
  // Eventing
  ConcurrentCircularBuffer<BufferedEvent> mEventQueue;
  static ConcurrentCircularBuffer<NextFrameFn> sExecutionQueue;
  volatile Vec2f mousePosBuf;
  
  // Path
  Path mCwd;
  
  // V8 Threads
  std::shared_ptr<std::thread> mV8Thread;
  std::shared_ptr<std::thread> mV8RenderThread;
  std::shared_ptr<std::thread> mV8EventThread;
  
  // Modules
  static v8::Persistent<v8::Object> sModuleCache;
  static v8::Persistent<v8::Array> sModuleList;
  
  // GC
  static void gcPrologueCb(Isolate *isolate, GCType type, GCCallbackFlags flags);
  static int sGCRuns;
  
  // Default Bindings
  static void setDrawCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void setEventCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toggleAppConsole(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toggleV8Stats(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void requestQuit(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void nextFrameJS(const v8::FunctionCallbackInfo<v8::Value>& args);
  
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
  
  // Quit
  static bool sQuitRequested;
};

#ifdef DEBUG
volatile bool CinderjsApp::sConsoleActive = true;
#else
volatile bool CinderjsApp::sConsoleActive = false;
#endif
volatile bool CinderjsApp::sV8StatsActive = false;

bool CinderjsApp::sQuitRequested = false;

v8::Persistent<v8::Object> CinderjsApp::sModuleCache;
v8::Persistent<v8::Array> CinderjsApp::sModuleList;
  
v8::Persistent<v8::Function> CinderjsApp::sDrawCallback;
v8::Persistent<v8::Function> CinderjsApp::sEventCallback;

v8::Persistent<v8::Object> CinderjsApp::sEmptyObject;

ConcurrentCircularBuffer<NextFrameFn> CinderjsApp::sExecutionQueue(1024);

int CinderjsApp::sGCRuns = 0;
void CinderjsApp::gcPrologueCb(Isolate *isolate, GCType type, GCCallbackFlags flags) {
  sGCRuns++;
  //AppConsole::log("GC Prologue " + std::to_string(sGCRuns));
  std::cout << "GC Prologue " << std::to_string(sGCRuns) << std::endl;
}

void CinderjsApp::handleV8TryCatch( v8::TryCatch &tryCatch ) {
  std::string msg;
  if(tryCatch.StackTrace().IsEmpty()){
    v8::String::Utf8Value except(tryCatch.Exception());
    msg = *except;
  } else {
    v8::String::Utf8Value trace(tryCatch.StackTrace());
    msg = *trace;
  }
  std::string ex = "JS Error: ";
  ex.append(msg);
  AppConsole::log( ex );
}


/**
 * Shutdown
 */
void CinderjsApp::shutdown()
{
  sConsoleActive = false;
  mShouldQuit = true;
  
  _v8Run = true;
  cvJSThread.notify_all();
  _eventRun = true;
  cvEventThread.notify_all();
  
  // Shutdown v8Thread
  if( mV8Thread ) {
    mV8Thread->join();
    mV8Thread.reset();
  }
  
  // Shutdown v8RenderThread
  if( mV8RenderThread ) {
    mV8RenderThread->join();
    mV8RenderThread.reset();
  }

  // Shutdown v8EventThread
  if( mV8EventThread ) {
    mV8EventThread->join();
    mV8EventThread.reset();
  }
  
  v8::V8::Dispose();
}

/**
 * Setup
 * If given a *.js file in the command line arguments, it will be run in the main context
 * and can be used to setup things for other scripts or to mockup the main app.
 */
void CinderjsApp::setup()
{
  // Init Console
  AppConsole::initialize();
  
  // Command line arguments
  vector<std::string> args = getArgs();

  mCwd = boost::filesystem::current_path();
  AppConsole::log("Current Path: " + mCwd.string());
  
  // Check argv arguments
  // TODO: Check for debug flag
  int pos = 0;
  for(std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
    pos++;
  }
  
  // clear out the window with black
  gl::clear( Color( 0, 0, 0 ) );
  
  glRenderer = getRenderer();
  
  // Get cinder.js main native
  if(cinder_native && sizeof(cinder_native) > 0) {
    std::string mainJS(cinder_native);
    mV8Thread = make_shared<std::thread>( boost::bind( &CinderjsApp::v8Thread, this, mainJS ) );
  } else {
    // FATAL: No main entry source
    quit();
  }
}

/**
 * Runs a JS string and prints eventual errors to the AppConsole
 */
v8::Local<v8::Value> CinderjsApp::executeScriptString( std::string scriptStr, Isolate* isolate, v8::Local<v8::Context> context, Handle<String> filename ){
  EscapableHandleScope scope(isolate);

  v8::TryCatch try_catch;
  
  // Enter the context for compiling and running
  Context::Scope context_scope(context);
  
  // Create a string containing the JavaScript source code.
  Local<String> source = String::NewFromUtf8( isolate, scriptStr.c_str() );
  
  // Compile the source code.
  Local<Script> script = Script::Compile( source, filename );
  
  // Run the script to get the result.
  Local<Value> result = script->Run();
  
  // Check for script errors
  if(try_catch.HasCaught()){
    handleV8TryCatch(try_catch);
  }
  
  return scope.Escape(result);
}

/**
 * Setup V8
 */
void CinderjsApp::v8Thread( std::string mainJS ){
  ThreadSetup threadSetup;
  
  // Initialize V8 (implicit initialization was removed in an earlier revision)
  v8::V8::InitializeICU();
  v8::Platform* platform = v8::platform::CreateDefaultPlatform(4);
  v8::V8::InitializePlatform(platform);
  V8::Initialize();
  
  // Create a new Isolate and make it the current one.
  mIsolate = Isolate::New();
  v8::Locker lock(mIsolate);
  Isolate::Scope isolate_scope(mIsolate);
  
  //mIsolate->AddGCPrologueCallback(gcPrologueCb);
  
  
  // Create a stack-allocated handle scope.
  HandleScope handle_scope(mIsolate);
  
  // Set general globals for JS
  mGlobal = ObjectTemplate::New();
  mGlobal->Set(v8::String::NewFromUtf8(mIsolate, "__draw__"), v8::FunctionTemplate::New(mIsolate, setDrawCallback));
  mGlobal->Set(v8::String::NewFromUtf8(mIsolate, "__event__"), v8::FunctionTemplate::New(mIsolate, setEventCallback));
  mGlobal->Set(v8::String::NewFromUtf8(mIsolate, "toggleAppConsole"), v8::FunctionTemplate::New(mIsolate, toggleAppConsole));
  mGlobal->Set(v8::String::NewFromUtf8(mIsolate, "toggleV8Stats"), v8::FunctionTemplate::New(mIsolate, toggleV8Stats));
  mGlobal->Set(v8::String::NewFromUtf8(mIsolate, "quit"), v8::FunctionTemplate::New(mIsolate, requestQuit));
  
  // Setup process object
  v8::Local<v8::ObjectTemplate> processObj = ObjectTemplate::New();
  processObj->Set(v8::String::NewFromUtf8(mIsolate, "nextFrame"), v8::FunctionTemplate::New(mIsolate, nextFrameJS));
  processObj->Set(v8::String::NewFromUtf8(mIsolate, "nativeBinding"), v8::FunctionTemplate::New(mIsolate, NativeBinding));
  // TODO: export process.platform
  // TODO: export process.noDeprecation
  // TODO: export process.throwDeprecation
  // TODO: export process.traceDeprecation
  // TODO: export process.pid
  // TODO: export process.env (environment vars etc.)
  //       -> Empty object for now (placeholer)
  processObj->Set(v8::String::NewFromUtf8(mIsolate, "env"), ObjectTemplate::New());
  //process.execPath
  processObj->Set(v8::String::NewFromUtf8(mIsolate, "execPath"), v8::String::NewFromUtf8(mIsolate, getAppPath().c_str()));
  mGlobal->Set(v8::String::NewFromUtf8(mIsolate, "process"), processObj);
  
  // TODO: export console[log, error, trace, etc.]
  
  //
  // Load Modules
  addModule(boost::shared_ptr<AppModule>( new AppModule() ));
  addModule(boost::shared_ptr<GLModule>( new GLModule() ));
  addModule(boost::shared_ptr<ConsoleModule>( new ConsoleModule() ));
  addModule(boost::shared_ptr<UtilsModule>( new UtilsModule() ));
  addModule(boost::shared_ptr<FSModule>( new FSModule() ));
  addModule(boost::shared_ptr<VMModule>( new VMModule() ));
  
  
  // Create a new context.
  v8::Local<v8::Context> mMainContext = Context::New(mIsolate, NULL, mGlobal);
  Context::Scope scope(mMainContext);
  pContext.Reset(mIsolate, mMainContext);
  
  // Get process object instance and set additional values
  Local<Value> processObjInstance = mMainContext->Global()->Get(v8::String::NewFromUtf8(mIsolate, "process"));
  
  // Initialize module list and set on process
  Local<Array> list = Array::New(mIsolate);
  sModuleList.Reset(mIsolate, list);
  processObjInstance.As<Object>()->Set(v8::String::NewFromUtf8(mIsolate, "modules"), list);
  
  // Initialize module cache
  Local<Object> obj = Object::New(mIsolate);
  sModuleCache.Reset(mIsolate, obj);
  processObjInstance.As<Object>()->Set(v8::String::NewFromUtf8(mIsolate, "_moduleCache"), obj);
  
  // add process.argv
  vector<std::string> argv = getArgs();
  
  #ifdef DEBUG
  // For development loading...
  //argv[2] = "/Users/sebastian/Dropbox/+Projects/cinderjs/lib/test.js";
  //argv.push_back("/Users/sebastian/Dropbox/+Projects/cinderjs/examples/particle.js");
  //argv.push_back("/Users/sebastian/Dropbox/+Projects/cinderjs/examples/lines.js");
  argv.push_back("/Users/sebastian/Dropbox/+Projects/cinderjs/examples/cubes.js");
  #endif
  
  Local<Array> argvArr = Array::New(mIsolate);
  for (int i = 0; i < argv.size(); ++i) {
    argvArr->Set(i, String::NewFromUtf8(mIsolate, argv[i].c_str()));
  }
  processObjInstance.As<Object>()->Set(v8::String::NewFromUtf8(mIsolate, "argv"), argvArr);
  
  // Setup global empty object for function callbacks
  Local<ObjectTemplate> emptyObj = ObjectTemplate::New();
  Handle<Object> emptyObjInstance = emptyObj->NewInstance();
  sEmptyObject.Reset(mIsolate, emptyObjInstance);
  
  // Grab GL context to execute the entry script and allow gl setup
  CGLContextObj currCtx = glRenderer->getCglContext();
  CGLSetCurrentContext( currCtx );
  CGLEnable( currCtx, kCGLCEMPEngine );
  CGLLockContext( currCtx );
  
  // Execute entry script
  Local<Value> mainResult = executeScriptString( mainJS, mIsolate,
    mMainContext, v8::String::NewFromUtf8(mIsolate, "cinder.js") );
  
  // Call wrapper function with process object
  if(mainResult->IsFunction()){
    Local<Function> mainFn = mainResult.As<Function>();
    
    v8::TryCatch tryCatch;
    
    mainFn->Call(mMainContext->Global(), 1, &processObjInstance);
    
    if(tryCatch.HasCaught()){
      handleV8TryCatch(tryCatch);
    }
    
    // TODO: Get Global()->global object and use in new context for external js modules
    
  } else {
    // FATAL: Main script not a function
    quit();
  }
  
  // Get rid of gl context again
  CGLUnlockContext( currCtx );
  CGLDisable( currCtx, kCGLCEMPEngine );
  
  // Start sub threads
  mV8RenderThread = make_shared<std::thread>( boost::bind( &CinderjsApp::v8RenderThread, this ) );
  mV8EventThread = make_shared<std::thread>( boost::bind( &CinderjsApp::v8EventThread, this ) );
}

/**
 *
 */
void CinderjsApp::v8RenderThread(){
  ThreadSetup threadSetup;
  
  CGLContextObj currCtx = glRenderer->getCglContext();
  
  // Not having the following two in seems to work fine as well and resizing works also
  //  CGLSetCurrentContext( currCtx );  //also important as it now sets newly created context for use in this thread
  //  CGLEnable( currCtx, kCGLCEMPEngine ); //Apple's magic sauce that allows this OpenGL context  to run in a thread
  
  double now;
  double lastFrame = getElapsedSeconds() * 1000;
  double timePassed;
  
  v8::HeapStatistics stats;
  
  //
  // Render Loop, do work if available
  while( !mShouldQuit ) {
    
    // Wait for data to be processed...
    {
        std::unique_lock<std::mutex> lck( mMainMutex );
        cvJSThread.wait(lck, [this]{ return _v8Run; });
    }
    
    if(!mShouldQuit){
      
      // Gather some info...
      now = getElapsedSeconds() * 1000;
      timePassed = now - lastFrame;
      lastFrame = now;
      double elapsed = now - mLastUpdate;
      if(elapsed > mUpdateInterval) {
        v8FPS = v8Frames;
        v8Frames = 0;
        mLastUpdate = now;
        if(sV8StatsActive) mIsolate->GetHeapStatistics(&stats);
      }
  
      CGLLockContext( currCtx ); //not sure if this is necessary but Apple's docs seem to suggest it
      glRenderer->startDraw();
      
      v8::Locker lock(mIsolate);
      
      // clear out the window with black
      gl::clear( Color( 0, 0, 0 ) );
      
      // JS Draw callback
      v8Draw( timePassed );
      
      // Draw modules
//      for( std::vector<boost::shared_ptr<PipeModule>>::iterator it = MODULES.begin(); it < MODULES.end(); ++it ) {
//        it->get()->draw();
//      }
      
      v8::Unlocker unlock(mIsolate);
      
      // FPS (TODO: if active)
      cinder::TextLayout fpsText;
      fpsText.setColor( cinder::ColorA( 1, 1, 1, 1 ) );
      fpsText.addLine( "Ci FPS: " + std::to_string( cinder::app::AppBasic::getAverageFps() ) );
      fpsText.addLine( "V8 FPS: " + std::to_string( v8FPS ) );
      
      if(sV8StatsActive){
        fpsText.addLine( "V8 Heap limit: " + std::to_string( stats.heap_size_limit() ) );
        fpsText.addLine( "V8 Heap total: " + std::to_string( stats.total_heap_size() ) );
        fpsText.addLine( "V8 Heap Used: " + std::to_string( stats.used_heap_size() ) );
      }
      
      cinder::gl::draw( cinder::gl::Texture( fpsText.render() ) );

      // Draw console (if active)
      if(sConsoleActive){
        Vec2f cPos;
        // TODO: this still fails sometimes on shutdown
        cPos.y = getWindowHeight();
        AppConsole::draw( cPos );
      }
      
      v8Frames++;
      
      glRenderer->finishDraw();
      CGLUnlockContext( currCtx );
    }
    
    _v8Run = false;
    _mainRun = true;
    cvMainThread.notify_one();
  }
  
  // FIXME: sometimes the threads just end shortly after startup already...
  //        Don't know how to reproduce.
  std::cout << "V8 Render thread ending" << std::endl;
  
  CGLDisable( currCtx, kCGLCEMPEngine );
  
}

/**
 *
 */
void CinderjsApp::v8EventThread(){
  ThreadSetup threadSetup;
  
  // Create a separte GL context to be used event callbacks that are invoked here
  CGLContextObj newCtx;
  
  CGDirectDisplayID display = CGMainDisplayID(); // 1
  CGOpenGLDisplayMask myDisplayMask = CGDisplayIDToOpenGLDisplayMask( display ); // 2
  
  // Check capabilities of display represented by display mask
  CGLPixelFormatAttribute attribs[] = { kCGLPFADisplayMask,
    (CGLPixelFormatAttribute)myDisplayMask,
    (CGLPixelFormatAttribute)0 }; // 3
  
  CGLPixelFormatObj pixelFormat = NULL;
  GLint numPixelFormats = 0;
  CGLChoosePixelFormat( attribs, &pixelFormat, &numPixelFormats );
  CGLCreateContext( pixelFormat, glRenderer->getCglContext(), &newCtx );
  CGLLockContext( newCtx );
  CGLSetCurrentContext( newCtx );
  CGLEnable( newCtx, kCGLCEMPEngine );
  
  // Thread loop
  while( !mShouldQuit ) {
    
    // Wait for data to be processed...
    {
        std::unique_lock<std::mutex> lck( mMainMutex );
        cvEventThread.wait(lck, [this]{ return _eventRun; });
    }
    
    if(!mShouldQuit && mEventQueue.isNotEmpty()){
      
      // TODO: If available, push mouse/key/resize events to v8
      v8::Locker lock(mIsolate);

      // Isolate
      v8::Isolate::Scope isolate_scope(mIsolate);
      v8::HandleScope handleScope(mIsolate);
      
      
      v8::Local<v8::Context> context = v8::Local<v8::Context>::New(mIsolate, pContext);
      
      if(context.IsEmpty()) return;
      
      Context::Scope ctxScope(context);
      context->Enter();
      
      
        
      while(mEventQueue.isNotEmpty()){
        BufferedEvent evt(new BufferedEventHolder());
        mEventQueue.popBack(&evt);
        
        // Callback
        v8::Local<v8::Function> callback;
        if(evt->type == CJS_NEXT_FRAME) {
          callback = v8::Local<v8::Function>::New(mIsolate, evt->v8Fn);
          evt->v8Fn.Reset(); // Get rid of persistent
        } else {
          callback = v8::Local<v8::Function>::New(mIsolate, sEventCallback);
        }
        
        if(callback.IsEmpty()) continue;
        
        // Resize Event
        if(evt->type == CJS_RESIZE){
          v8::Handle<v8::Value> argv[3] = {
            v8::Number::New(mIsolate, CJS_RESIZE),
            v8::Number::New(mIsolate, getWindowWidth()),
            v8::Number::New(mIsolate, getWindowHeight())
          };
          callback->Call(context->Global(), 3, argv);
        }
        
        // Key down
        else if(evt->type == CJS_KEY_DOWN){
          v8::Handle<v8::Value> argv[3] = {
            v8::Number::New(mIsolate, CJS_KEY_DOWN),
            v8::Number::New(mIsolate, evt->kEvt.getCode()),
            v8::Number::New(mIsolate, evt->kEvt.getChar())
            // ... TODO: Push more event info
          };
          callback->Call(context->Global(), 3, argv);
        }
        
        // Key up
        else if(evt->type == CJS_KEY_UP){
          v8::Handle<v8::Value> argv[3] = {
            v8::Number::New(mIsolate, CJS_KEY_UP),
            v8::Number::New(mIsolate, evt->kEvt.getCode()),
            v8::Number::New(mIsolate, evt->kEvt.getChar())
            // ... TODO: Push more event info
          };
          callback->Call(context->Global(), 3, argv);
        }
        
        // Next Frame
        else if(evt->type == CJS_NEXT_FRAME){
          v8::Handle<v8::Value> argv[0] = {};
          callback->Call(context->Global(), 0, argv);
        }
        
        // Shutdown (gracefully)
        else if(evt->type == CJS_SHUTDOWN_REQUEST){
          quit();
        }

      }
      
      context->Exit();
      v8::Unlocker unlock(mIsolate);
    }
    
    _eventRun = false;
  }
  
  CGLUnlockContext( newCtx );
  CGLDisable( newCtx, kCGLCEMPEngine );
  CGLDestroyContext( newCtx );
  
  std::cout << "V8 Event thread ending" << std::endl;
}

/**
 *
 */
void CinderjsApp::resize()
{
  BufferedEvent evt(new BufferedEventHolder());
  evt->type = CJS_RESIZE;
  mEventQueue.pushFront(evt);
  _eventRun = true;
  cvEventThread.notify_one();
}

/**
 *
 */
void CinderjsApp::mouseMove( MouseEvent event )
{
  // Update mouse position (pushed to v8 with draw callback)
  mousePosBuf.x = event.getX();
  mousePosBuf.y = event.getY();
}

/**
 *
 */
void CinderjsApp::mouseDown( MouseEvent event )
{
  // TODO: Use event thread to push event to v8
  //  _eventRun = true;
  //  cvEventThread.notify_one();
}

/**
 * Key down
 */
void CinderjsApp::keyDown( KeyEvent event )
{
  BufferedEvent evt(new BufferedEventHolder());
  evt->type = CJS_KEY_DOWN;
  evt->kEvt = event;
  mEventQueue.pushFront(evt);
  _eventRun = true;
  cvEventThread.notify_one();
}

/**
 * Key up
 */
void CinderjsApp::keyUp( KeyEvent event )
{
  BufferedEvent evt(new BufferedEventHolder());
  evt->type = CJS_KEY_UP;
  evt->kEvt = event;
  mEventQueue.pushFront(evt);
  _eventRun = true;
  cvEventThread.notify_one();
}

/**
 *
 */
void CinderjsApp::update()
{
  
}

/**
 * Cinder draw loop
 * Triggers the v8 render thread
 */
void CinderjsApp::draw()
{
  
  // Trigger v8 draw if not running already...
  if(!_v8Run){
    // Quit if requested
    // Push to event queue and execute there (not check every frame, duh)
    if(sQuitRequested) {
      BufferedEvent evt(new BufferedEventHolder());
      evt->type = CJS_SHUTDOWN_REQUEST;
      mEventQueue.pushFront(evt);
      return;
    }
    _v8Run = true;
    cvJSThread.notify_one();
  }
  
  // Handle execution queue
  if(sExecutionQueue.isNotEmpty()){
    while(sExecutionQueue.isNotEmpty()){
      BufferedEvent evt(new BufferedEventHolder());
      NextFrameFn nffn(new NextFrameFnHolder());
      sExecutionQueue.popBack(&nffn);
      evt->type = CJS_NEXT_FRAME;
      evt->v8Fn.Reset(mIsolate, nffn->v8Fn);
      mEventQueue.pushFront(evt);
    }
    _eventRun = true;
    cvEventThread.notify_one();
  }
  
}

/**
 * V8 Draw
 * Called by v8 render thread
 */
void CinderjsApp::v8Draw( double timePassed ){
  v8::Locker lock(mIsolate);
  
  // Isolate
  v8::Isolate::Scope isolate_scope(mIsolate);
  v8::HandleScope handleScope(mIsolate);
  
  // Callback
  v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(mIsolate, sDrawCallback);
  
  v8::Local<v8::Context> context = v8::Local<v8::Context>::New(mIsolate, pContext);
  
  if(context.IsEmpty()) return;
  
  Context::Scope ctxScope(context);
  context->Enter();
  
  if( !callback.IsEmpty() ){
    
    v8::Handle<v8::Value> argv[3] = {
      v8::Number::New(mIsolate, timePassed),
      v8::Number::New(mIsolate, mousePosBuf.x),
      v8::Number::New(mIsolate, mousePosBuf.y)
    };

    v8::TryCatch try_catch;

    Local<Object> emptyObj = Local<Object>::New(mIsolate, sEmptyObject);

    //callback->Call(context->Global(), 3, argv);
    callback->Call(emptyObj, 3, argv);
    
    // Check for errors
    if(try_catch.HasCaught()){
      handleV8TryCatch(try_catch);
    }

    callback.Clear();
    argv->Clear();
  }
  
  context->Exit();
}

/**
 * Set the draw callback from javascript
 */
void CinderjsApp::setDrawCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Locker lock(isolate);
  v8::HandleScope handleScope(isolate);
  
  if(!args[0]->IsFunction()){
    // throw js exception
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "draw callback expects one argument of type function."));
    return;
  }
  AppConsole::log("draw callback set.");
  
  sDrawCallback.Reset(isolate, args[0].As<v8::Function>());
  
  // TODO: strip drawCallback from global obj, so only one main loop can be used
  
  return;
}

/**
 * Set event callback from javascript to push mouse/key events to
 */
void CinderjsApp::setEventCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Locker lock(isolate);
  v8::HandleScope handleScope(isolate);
  
  if(!args[0]->IsFunction()){
    // throw js exception
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "event callback expects one argument of type function."));
    return;
  }
  AppConsole::log("event callback set.");
  
  sEventCallback.Reset(isolate, args[0].As<v8::Function>());
  
  // TODO: Remove event callback setter function from global context
  
  return;
}

/**
 * Toggle the AppConsole view on/off
 */
void CinderjsApp::toggleAppConsole(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if(args.Length() > 0) {
    bool active = args[0]->ToBoolean()->Value();
    sConsoleActive = active;
    return;
  }

  sConsoleActive = !sConsoleActive;
  return;
}

/**
 * Toggle v8 stats on/off
 */
void CinderjsApp::toggleV8Stats(const v8::FunctionCallbackInfo<v8::Value>& args) {
  sV8StatsActive = !sV8StatsActive;
  return;
}

/**
 * Allow JS to quit the App
 */
void CinderjsApp::requestQuit(const v8::FunctionCallbackInfo<v8::Value>& args) {
  sQuitRequested = true;
  // TODO: emit process exit event for js to react on and shutdown stuff if necessary
  return;
}

/**
 * Async function execution in the next frame (executed in v8 event thread)
 */
void CinderjsApp::nextFrameJS(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  if(args[0]->IsFunction()){
    NextFrameFn nffn(new NextFrameFnHolder());
    nffn->v8Fn.Reset(isolate, args[0].As<Function>());
    sExecutionQueue.pushFront(nffn);
  }
}

/**
 * Load a native js module (lib)
 */
void CinderjsApp::NativeBinding(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope handle_scope(isolate);
  
  Locker lock(isolate);
  
  Local<String> moduleName = args[0]->ToString();
  
  if (moduleName.IsEmpty()) {
    std::string except = "NativeBinding needs a module name. fail.";
    isolate->ThrowException(String::NewFromUtf8(isolate, except.c_str()));
    return;
  }
  
  // if we have a second argument and it is a boolean true,
  // check for module existence only and return a boolean that can be checked with === operator
  bool checkExistence = false;
  if( args.Length() == 2 && args[1]->ToBoolean()->Value() ) {
    checkExistence = true;
  }
  
  Local<Object> cache = Local<Object>::New(isolate, sModuleCache);
  Local<Object> exports;

  if (cache->Has(moduleName)) {
    exports = cache->Get(moduleName)->ToObject();
    args.GetReturnValue().Set(exports);
    return;
  }
  
  v8::String::Utf8Value strModuleName(moduleName);
  std::string cmpModName(*strModuleName);
  
  // Add to list
  std::string buf = "Native ";
  buf.append(*strModuleName);
  
  Local<Array> moduleList = Local<Array>::New(isolate, sModuleList);
  uint32_t len = moduleList->Length();
  moduleList->Set(len, String::NewFromUtf8(isolate, buf.c_str()));
  
  // Lookup native modules
  _native mod;
  bool found = false;
  for(int i = 0; i < sizeof(natives); i++) {
    if(natives[i].name == NULL) break;
    std::string cmp(natives[i].name);
    if(cmp == "cinder") continue;
    if(cmp == cmpModName){
      mod = natives[i];
      found = true;
      break;
    }
  }
  
  // If we only have to check existence, we return here
  if( checkExistence ) {
    args.GetReturnValue().Set(v8::Boolean::New(isolate, found));
    return;
  }
  
  if (found) {
  
    exports = Object::New(isolate);
    
    v8::TryCatch tryCatch;
    
    // wrappers and wrap method set in cinder.js js land
    Local<Function> wrap = isolate
      ->GetCurrentContext()
      ->Global()
      ->Get(v8::String::NewFromUtf8(isolate, "process"))
      .As<Object>()
      ->Get(v8::String::NewFromUtf8(isolate, "wrap"))
      .As<Function>()
    ; // wtf v8...
    
    // Check v8 in sanity
    if(!wrap->IsFunction()) {
      std::string except = "Native module loading failed: No wrap method on the process object.";
      isolate->ThrowException(String::NewFromUtf8(isolate, except.c_str()));
      return;
    }
    
    // Wrap it
    Local<Value> modSource = String::NewFromUtf8(isolate, mod.source);
    v8::String::Utf8Value wrappedSource( wrap->Call(Local<Object>::New(isolate, sEmptyObject), 1, &modSource) );
    std::string filename = cmpModName;
    filename.append(".js");
    Local<Value> modResult = executeScriptString(*wrappedSource, isolate, isolate->GetCurrentContext(),
      v8::String::NewFromUtf8(isolate, filename.c_str()) );
    
    // Check native module validity
    if(modResult.IsEmpty() || !modResult->IsFunction()){
      std::string except = "Native module not a function: ";
      except.append(*strModuleName);
    
      isolate->ThrowException(String::NewFromUtf8(isolate, except.c_str()));
      // TODO: remove from module list again
      return;
    }
    
    Local<Function> modFn = modResult.As<Function>();
    
    // Call
    Local<Value> argv[2] = {
      exports,
      // use this method as require as native modules won't require external ones(?)
      v8::Local<v8::Value>::Cast(v8::FunctionTemplate::New(isolate, NativeBinding)->GetFunction())
    };
    
    // Try: v8::Local<Context>::New(isolate, pContext)
    modFn->Call(isolate->GetCallingContext()->Global(), 2, argv);
    
    if(tryCatch.HasCaught()){
      handleV8TryCatch(tryCatch);
      // TODO: remove from module list again
      return;
    }
    
    cache->Set(moduleName, exports);
  } else {
    std::string except = "No such native module: ";
    except.append(*strModuleName);
  
    isolate->ThrowException(String::NewFromUtf8(isolate, except.c_str()));
    // TODO: remove from module list again
    return;
  }

  args.GetReturnValue().Set(exports);
}
  
} // namespace cjs

CINDER_APP_NATIVE( cjs::CinderjsApp, RendererGl )
