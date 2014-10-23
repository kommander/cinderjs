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

#include "vm.hpp"
#include "AppConsole.h"

#include "cinder/Filesystem.h"

using namespace std;
using namespace cinder;
using namespace v8;

namespace cjs {

void runInThisContext(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  
  // Enter the context for compiling and running
  Context::Scope context_scope(isolate->GetCallingContext());
  
  // Create a string containing the JavaScript source code.
  Local<String> source = args[0]->ToString();
  Local<String> filename = args[1]->ToString();
  
  // Debug
  //String::Utf8Value str(source);
  //std::cout << "runInThisContext: " << std::endl << *str << std::endl;
  
  // Compile the source code.
  Local<Script> script = Script::Compile( source, filename );
  
  // Run the script to get the result.
  TryCatch try_catch;
  
  Local<Value> result = script->Run();
  
  // Check native module validity
  if(result.IsEmpty() || !result->IsFunction()){
    std::string except = "External module not a function, in ";
    v8::String::Utf8Value fname(filename);
    except.append(*fname);
    
    if(try_catch.HasCaught()){
      v8::String::Utf8Value msg(try_catch.Exception());
      except.append(*msg);
    }
    
    isolate->ThrowException(String::NewFromUtf8(isolate, except.c_str()));
    // TODO: remove from module list again
    return;
  }
    
  args.GetReturnValue().Set(result);
  
  return;
}

/**
 * Add JS bindings
 */
void VMModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global vm object
  Handle<ObjectTemplate> vmTemplate = ObjectTemplate::New(getIsolate());
  
  vmTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "runInThisContext"), v8::FunctionTemplate::New(getIsolate(), runInThisContext));
  
  // Expose global fs object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "_vm"), vmTemplate);
}
 
} // namespace cjs