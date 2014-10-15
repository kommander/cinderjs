#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <string.h>

#include "v8.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace v8;

class cinderjsApp : public AppNative {
  public:
  ~cinderjsApp(){
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
  
  private:
  std::shared_ptr<std::thread> mV8Thread;
  
  static void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void test(const v8::FunctionCallbackInfo<v8::Value>& args);
};

void cinderjsApp::LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  
  v8::String::Utf8Value str(args[0]);
  printf("%s", *str);
  
  return;
}

void cinderjsApp::test(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  
  ReturnValue<v8::Value> ret = args.GetReturnValue();
  ret.Set(v8::String::NewFromUtf8(isolate, "Hello World!"));
}

void cinderjsApp::setup()
{
  mV8Thread = make_shared<std::thread>( boost::bind( &cinderjsApp::v8Thread, this ) );
}

void cinderjsApp::v8Thread(){
  ThreadSetup threadSetup;
  
  V8::Initialize();
  
  // Create a new Isolate and make it the current one.
  Isolate* isolate = Isolate::New();
  Isolate::Scope isolate_scope(isolate);
  
  // Create a stack-allocated handle scope.
  HandleScope handle_scope(isolate);
  
  Local<ObjectTemplate> global = ObjectTemplate::New();
  global->Set(v8::String::NewFromUtf8(isolate, "log"), FunctionTemplate::New(isolate, LogCallback));
  global->Set(v8::String::NewFromUtf8(isolate, "test"), FunctionTemplate::New(isolate, test));
  
  // Create a new context.
  Local<Context> context = Context::New(isolate, NULL, global);
  
  // Enter the context for compiling and running the hello world script.
  Context::Scope context_scope(context);
  
  // Create a string containing the JavaScript source code.
  Local<String> source = String::NewFromUtf8(isolate, "log(test())");
  
  // Compile the source code.
  Local<Script> script = Script::Compile(source);
  
  // Run the script to get the result.
  Local<Value> result = script->Run();
}

void cinderjsApp::mouseDown( MouseEvent event )
{
}

void cinderjsApp::update()
{
}

void cinderjsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( cinderjsApp, RendererGl )
