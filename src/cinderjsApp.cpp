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


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace v8;

namespace cjs {

typedef boost::filesystem::path Path;

struct BufferedEvent {
  int type;
  MouseEvent mEvt;
  KeyEvent kEvt;
};

enum EventType {
  CJS_RESIZE = 10,
  CJS_KEY_DOWN = 20,
  CJS_KEY_UP = 30
};

// TODO
// - Split cinderjsApp in header file
// - load js modules with wrapper: "function (module, exports, __filename, ...) {"
// - Expose versions object (cinder, v8, cinderjs)

class CinderjsApp : public AppNative, public CinderAppBase  {
  public:
  CinderjsApp() : mEventQueue(32) {}
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
  void runJS( std::string scriptStr );
  Local<Context> createMainContext(Isolate* isolate);
  
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
  volatile Vec2f mousePosBuf;
  
  // Path
  Path mCwd;
  
  // V8
  std::shared_ptr<std::thread> mV8Thread;
  std::shared_ptr<std::thread> mV8RenderThread;
  std::shared_ptr<std::thread> mV8EventThread;
  
  // GC
  static void gcPrologueCb(Isolate *isolate, GCType type, GCCallbackFlags flags);
  static int sGCRuns;
  
  // Default Bindings
  static void setDrawCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void setEventCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toggleAppConsole(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toggleV8Stats(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void requestQuit(const v8::FunctionCallbackInfo<v8::Value>& args);

  // Default Callbacks
  static v8::Persistent<v8::Function> sDrawCallback;
  static v8::Persistent<v8::Function> sEventCallback; // TODO: only push events that were subscribed to in v8
  
  // Console
  static volatile bool sConsoleActive;
  static volatile bool sV8StatsActive;
  
  // Quit
  static bool sQuitRequested;
};

volatile bool CinderjsApp::sConsoleActive = true;
volatile bool CinderjsApp::sV8StatsActive = true;

bool CinderjsApp::sQuitRequested = false;

v8::Persistent<v8::Function> CinderjsApp::sDrawCallback;
v8::Persistent<v8::Function> CinderjsApp::sEventCallback;

int CinderjsApp::sGCRuns = 0;
void CinderjsApp::gcPrologueCb(Isolate *isolate, GCType type, GCCallbackFlags flags) {
  sGCRuns++;
  //AppConsole::log("GC Prologue " + std::to_string(sGCRuns));
  std::cout << "GC Prologue " << std::to_string(sGCRuns) << std::endl;
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
  
  // TODO: Choose between loading a script from asset folder or specified in command line
  #ifdef DEBUG
  //std::string jsMainFile = "/Users/sebastian/Dropbox/+Projects/cinderjs/lib/test.js";
  std::string jsMainFile = "/Users/sebastian/Dropbox/+Projects/cinderjs/examples/particle.js";
  //std::string jsMainFile = "/Users/sebastian/Dropbox/+Projects/cinderjs/examples/lines.js";
  #else
  std::string jsMainFile;
  #endif
  
  // Check argv arguments
  int pos = 0;
  for(std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
    AppConsole::log("argv " + std::to_string(pos) + ":" + *it);
    pos++;
   
    std::string s = *it;
    if( s.find(".js") != std::string::npos ){
      jsMainFile = *it;
    }
  }
  
  // Do we have a js file to run?
  std::string jsFileContents;
  if(jsMainFile.length() > 0) {
    AppConsole::log("Starting app with JS file at: " + jsMainFile);
    
    if( !cinder::fs::exists( jsMainFile ) ){
      AppConsole::log("Could not find specified JS file!");
    } else {
      try {
        std::ifstream in(jsMainFile, std::ios::in );
        if (in)
        {
          std::ostringstream contents;
          contents << in.rdbuf();
          jsFileContents = contents.str();
          in.close();
        }
      } catch(std::exception &e) {
        std::string err = "Error: ";
        err.append(e.what());
        AppConsole::log( err );
      }
    }
  }
  
  // clear out the window with black
  gl::clear( Color( 0, 0, 0 ) );
  
  glRenderer = getRenderer();
  
  //CGLContextObj currCtx = CGLGetCurrentContext();
  mV8Thread = make_shared<std::thread>( boost::bind( &CinderjsApp::v8Thread, this, jsFileContents ) );
  
}

/**
 *
 */
Local<Context> CinderjsApp::createMainContext(Isolate* isolate) {
  EscapableHandleScope handleScope(isolate);
  
  Local<Context> ct = Context::New(mIsolate, NULL, mGlobal);
  Context::Scope scope(ct);
  pContext.Reset(isolate, ct);
  
  return handleScope.Escape(ct);
}

/**
 * Runs a JS string and prints eventual errors to the AppConsole
 */
void CinderjsApp::runJS( std::string scriptStr ){
  v8::TryCatch try_catch;
  
  // Enter the context for compiling and running the hello world script.
  Context::Scope context_scope(mMainContext);
  
  // Create a string containing the JavaScript source code.
  Local<String> source = String::NewFromUtf8( mIsolate, scriptStr.c_str() );
  
  // Compile the source code.
  Local<Script> script = Script::Compile( source );
  
  // Run the script to get the result.
  Local<Value> result = script->Run();
  
  // Check for script errors
  if(result.IsEmpty()){
    if(try_catch.HasCaught()){
      //v8::String::Utf8Value exception(try_catch.Exception());
      v8::String::Utf8Value trace(try_catch.StackTrace());
      std::string ex = "JS Error: ";
      //ex.append(*exception);
      ex.append(*trace);
      AppConsole::log( ex );
    }
  }
  
}

/**
 *
 */
void CinderjsApp::v8Thread( std::string jsFileContents ){
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
  
  //
  // Load Modules
  addModule(boost::shared_ptr<AppModule>( new AppModule() ));
  addModule(boost::shared_ptr<GLModule>( new GLModule() ));
  addModule(boost::shared_ptr<ConsoleModule>( new ConsoleModule() ));
  
  
  // Create a new context.
  //mMainContext = Context::New(mIsolate, NULL, mGlobal);
  mMainContext = createMainContext(mIsolate);
  
  if( jsFileContents.length() > 0 ){
    runJS( jsFileContents );
  }

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
      fpsText.addRightLine( "FPS: " + std::to_string( cinder::app::AppBasic::getAverageFps() ) );
      fpsText.addRightLine( "v8FPS: " + std::to_string( v8FPS ) );
      
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
      
      // Callback
      // TODO: initialize on thread start and reuse
      v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(mIsolate, sEventCallback);
      
      v8::Local<v8::Context> context = v8::Local<v8::Context>::New(mIsolate, pContext);
      
      if(context.IsEmpty()) return;
      
      Context::Scope ctxScope(context);
      context->Enter();
      
      if( !callback.IsEmpty() ){
        
        while(mEventQueue.isNotEmpty()){
          BufferedEvent evt;
          mEventQueue.popBack(&evt);
          
          // Resize Event
          if(evt.type == CJS_RESIZE){
            v8::Handle<v8::Value> argv[3] = {
              v8::Number::New(mIsolate, CJS_RESIZE),
              v8::Number::New(mIsolate, getWindowWidth()),
              v8::Number::New(mIsolate, getWindowHeight())
            };
            callback->Call(context->Global(), 3, argv);
            argv->Clear();
          }
          
          // Key down
          else if(evt.type == CJS_KEY_DOWN){
            v8::Handle<v8::Value> argv[3] = {
              v8::Number::New(mIsolate, CJS_KEY_DOWN),
              v8::Number::New(mIsolate, evt.kEvt.getCode()),
              v8::Number::New(mIsolate, evt.kEvt.getChar())
              // ... TODO: Push more event info
            };
            callback->Call(context->Global(), 3, argv);
            argv->Clear();
          }
          
          // Key up
          else if(evt.type == CJS_KEY_UP){
            v8::Handle<v8::Value> argv[3] = {
              v8::Number::New(mIsolate, CJS_KEY_UP),
              v8::Number::New(mIsolate, evt.kEvt.getCode()),
              v8::Number::New(mIsolate, evt.kEvt.getChar())
              // ... TODO: Push more event info
            };
            callback->Call(context->Global(), 3, argv);
            argv->Clear();
          }
        
        }
        
        callback.Clear();
      }
      
      context->Exit();
      v8::Unlocker unlock(mIsolate);
    }
    
    _eventRun = false;
  }
  
  std::cout << "V8 Event thread ending" << std::endl;
}

/**
 *
 */
void CinderjsApp::resize()
{
  BufferedEvent evt;
  evt.type = CJS_RESIZE;
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
  BufferedEvent evt;
  evt.type = CJS_KEY_DOWN;
  evt.kEvt = event;
  mEventQueue.pushFront(evt);
  _eventRun = true;
  cvEventThread.notify_one();
}

/**
 * Key up
 */
void CinderjsApp::keyUp( KeyEvent event )
{
  BufferedEvent evt;
  evt.type = CJS_KEY_UP;
  evt.kEvt = event;
  mEventQueue.pushFront(evt);
  _eventRun = true;
  cvEventThread.notify_one();
}

/**
 *
 */
void CinderjsApp::update()
{
  // Quit if requested
  if(sQuitRequested) {
    quit();
  }
}

/**
 * Cinder draw loop
 * Triggers the v8 render thread
 */
void CinderjsApp::draw()
{
  
  // Trigger v8 draw if not running already...
  if(!_v8Run){
    _v8Run = true;
    cvJSThread.notify_one();
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
    
    callback->Call(context->Global(), 3, argv);
    
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
  return;
}
  
} // namespace cjs

CINDER_APP_NATIVE( cjs::CinderjsApp, RendererGl )
