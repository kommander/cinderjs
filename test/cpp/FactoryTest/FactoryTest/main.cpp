//
//  main.cpp
//  FactoryTest
//
//  Created by Sebastian Herrlinger on 31/10/14.
//  Copyright (c) 2014 kosmokommando. All rights reserved.
//

#include <iostream>
#include "v8.h"
#include "libplatform/libplatform.h"

#include "StaticFactory.hpp"

#include "boost/shared_ptr.hpp"

using namespace v8;
using namespace cjs;

// Test class
class Material {
  public:
  Material(){}
  ~Material(){}
};

void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  
  v8::String::Utf8Value str(args[0]);

  std::cout << *str << std::endl;
  std::cout.flush();

  return;
}

void addMaterial(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  
  Local<Object> idHolder = StaticFactory::put<Material>(isolate, boost::shared_ptr<Material>(new Material()));
  
  assert(idHolder->IsObject());
  assert(idHolder->Has(v8::String::NewFromUtf8(isolate, "id")));
  
  //std::cout << "Creating Material" << std::endl;
  
  args.GetReturnValue().Set(idHolder);
  return;
}

void destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  
  uint32_t id = args[0]->ToUint32()->Value();
  StaticFactory::remove<Material>(isolate, id);
  
  return;
}

std::string testScript = "                                                  \
  log('Starting');                                                          \
     var mat;                                                               \
     for(var i = 0; i < 20000; i++){                                        \
       mat = addMaterial();                                                 \
       if(mat === false){                                                   \
         log('Material could not be created');                              \
       }                                                                    \
       destroy(mat.id);                                                     \
     }                                                                      \
  log('done');                                                              \
";

void runScript(Isolate* isolate, Handle<Context> ctx, std::string scriptStr){
  Isolate::Scope isolate_scope(isolate);
  
  // Enter the context for compiling and running the hello world script.
  Context::Scope context_scope(ctx);
  
  // Create a string containing the JavaScript source code.
  Local<String> source = String::NewFromUtf8(isolate, scriptStr.c_str());
  
  // Compile the source code.
  Local<Script> script = Script::Compile(source);
  
  // Run the script to get the result.
  Local<Value> result = script->Run();
}

int main(int argc, const char * argv[]) {
  v8::V8::InitializeICU();
  v8::Platform* platform = v8::platform::CreateDefaultPlatform(4);
  v8::V8::InitializePlatform(platform);
  V8::Initialize();
  
  StaticFactory::initialize();
  
  // Create a new Isolate and make it the current one.
  Isolate* isolate = Isolate::New();
  Isolate::Scope isolate_scope(isolate);
  
  // Create a stack-allocated handle scope.
  HandleScope handle_scope(isolate);
  
  Local<ObjectTemplate> global = ObjectTemplate::New();
  global->Set(v8::String::NewFromUtf8(isolate, "log"), FunctionTemplate::New(isolate, LogCallback));
  global->Set(v8::String::NewFromUtf8(isolate, "addMaterial"), FunctionTemplate::New(isolate, addMaterial));
  global->Set(v8::String::NewFromUtf8(isolate, "destroy"), FunctionTemplate::New(isolate, destroy));
  
  // Create a new context.
  Local<Context> context = Context::New(isolate, NULL, global);
  
  //
  //
  runScript(isolate, context, testScript);
  
  std::cout << "First run Factory size: " << std::to_string(StaticFactory::size()) << std::endl;
  std::cout << std::to_string(StaticFactory::getStats().puts) << " puts / "
    << std::to_string(StaticFactory::getStats().removes) << " removes" << std::endl;
  std::cout.flush();
  sleep(3);
  
  while(!v8::IDLE){
    //wait.
  }
  
  //
  //
  runScript(isolate, context, testScript);
  
  std::cout << "Second run Factory size: " << std::to_string(StaticFactory::size()) << std::endl;
  std::cout << std::to_string(StaticFactory::getStats().puts) << " puts / "
    << std::to_string(StaticFactory::getStats().removes) << " removes" << std::endl;
  std::cout.flush();
  sleep(3);
  
  while(!v8::IDLE){
    //wait.
  }
  
  //
  //
  runScript(isolate, context, testScript);
  
  std::cout << "Third run Factory size: " << std::to_string(StaticFactory::size()) << std::endl;
  std::cout << std::to_string(StaticFactory::getStats().puts) << " puts / "
    << std::to_string(StaticFactory::getStats().removes) << " removes" << std::endl;
  std::cout.flush();
  sleep(3);
  
  while(!v8::IDLE){
    //wait.
  }
  
  v8::V8::Dispose();
  std::cout << "After Dispose Factory size: " << std::to_string(StaticFactory::size()) << std::endl;
  std::cout << std::to_string(StaticFactory::getStats().puts) << " puts / "
    << std::to_string(StaticFactory::getStats().removes) << " removes" << std::endl;
  
  return 0;
}
