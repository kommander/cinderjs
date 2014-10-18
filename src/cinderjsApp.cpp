#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Filesystem.h"

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
  
class CinderjsApp : public AppNative, public CinderAppBase  {
  public:
  ~CinderjsApp(){
    v8::V8::Dispose();
    
    // Shutdown v8Thread
    if( mV8Thread ) {
      mV8Thread->join();
      mV8Thread.reset();
    }
  }
  
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
  
  void v8Thread();
  void runJS( std::string scriptStr );
  Local<Context> createMainContext(Isolate* isolate);
  
  private:
  
  // Path
  Path mCwd;
  
  // V8
  std::shared_ptr<std::thread> mV8Thread;
  
  //static void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void drawCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

};

/**
 * Setup
 * If given a *.js file in the command line arguments, it will be run in the main context
 * and can be used to setup things for other scripts or to mockup the main app.
 */
void CinderjsApp::setup()
{
  // Init Console
  AppConsole::initialize();
  
  mV8Thread = make_shared<std::thread>( boost::bind( &CinderjsApp::v8Thread, this ) );
  
  // Command line arguments
  vector<std::string> args = getArgs();

  mCwd = boost::filesystem::current_path();
  AppConsole::log("Current Path: " + mCwd.string());
  
  // TODO: Choose between loading a script from asset folder or specified in command line
  #ifdef DEBUG
  std::string jsMainFile = "/Users/sebastian/Dropbox/+Projects/cinderjs/lib/test.js";
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
  
  // Initialize V8 (implicit initialization was removed in an earlier revision)
  V8::Initialize();
  
  // Create a new Isolate and make it the current one.
  mIsolate = Isolate::New();
  Isolate::Scope isolate_scope(mIsolate);
  
  // Create a stack-allocated handle scope.
  HandleScope handle_scope(mIsolate);
  
  // Set general globals for JS
  mGlobal = ObjectTemplate::New();
  
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
}

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

void CinderjsApp::v8Thread(){
  ThreadSetup threadSetup;
}

void CinderjsApp::mouseDown( MouseEvent event )
{
}

void CinderjsApp::update()
{
}

void CinderjsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
  
  // Draw modules
  for( std::vector<boost::shared_ptr<PipeModule>>::iterator it = MODULES.begin(); it < MODULES.end(); ++it ) {
    it->get()->draw();
  }
  
  // Draw console (TODO: if active)
  Vec2f cPos;
  cPos.y = getWindowHeight();
  AppConsole::draw( cPos );
}
  
} // namespace cjs

CINDER_APP_NATIVE( cjs::CinderjsApp, RendererGl )
