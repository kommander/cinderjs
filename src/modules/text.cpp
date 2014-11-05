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

#include "text.hpp"
#include "AppConsole.h"

using namespace std;
using namespace cinder;
using namespace v8;

namespace cjs {

// TODO: Use StaticFactory
std::map<uint32_t, std::shared_ptr<SimpleText>> TextModule::sTextObjects;
uint32_t TextModule::sTextObjectIds = 0;
vec3 TextModule::bufVec3f_1;

void TextModule::createSimpleText(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  
  v8::String::Utf8Value str(args[0]);
  
  uint32_t id = ++sTextObjectIds;
  
  std::shared_ptr<SimpleText> newText( new SimpleText() );
  newText->setText( *str );
  
  sTextObjects[id] = newText;
  
  args.GetReturnValue().Set(v8::Uint32::New(isolate, id));
}

void TextModule::drawSimpleText(const v8::FunctionCallbackInfo<v8::Value>& args) {
  uint32_t id = args[0]->Uint32Value();
  std::shared_ptr<SimpleText> textObj = sTextObjects[id];
  if( textObj ){
    textObj->draw();
  }
}

void TextModule::updateSimpleText(const v8::FunctionCallbackInfo<v8::Value>& args) {
  uint32_t id = args[0]->Uint32Value();
  std::shared_ptr<SimpleText> textObj = sTextObjects[id];
  if( textObj ){
    v8::String::Utf8Value str(args[1]);
    textObj->setText( *str );
  }
}

void TextModule::setSimpleTextPos(const v8::FunctionCallbackInfo<v8::Value>& args) {
  uint32_t id = args[0]->Uint32Value();
  std::shared_ptr<SimpleText> textObj = sTextObjects[id];
  if( textObj ){
    bufVec3f_1.x = args[1]->NumberValue();
    bufVec3f_1.y = args[2]->NumberValue();
    bufVec3f_1.z = args[3]->NumberValue();
    textObj->setPosition(bufVec3f_1);
  }
}

/**
 * Load bindings onto global js object
 */
void TextModule::loadGlobalJS( v8::Local<v8::ObjectTemplate> &global ) {
  // Create global text object
  Handle<ObjectTemplate> utilsTemplate = ObjectTemplate::New(getIsolate());
  
  utilsTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "createSimpleText"), v8::FunctionTemplate::New(getIsolate(), createSimpleText));
  utilsTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "drawSimpleText"), v8::FunctionTemplate::New(getIsolate(), drawSimpleText));
  utilsTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "updateSimpleText"), v8::FunctionTemplate::New(getIsolate(), updateSimpleText));
  utilsTemplate->Set(v8::String::NewFromUtf8(getIsolate(), "setSimpleTextPosition"), v8::FunctionTemplate::New(getIsolate(), setSimpleTextPos));
  
  // Expose global text object
  global->Set(v8::String::NewFromUtf8(getIsolate(), "text"), utilsTemplate);
}

 
} // namespace cjs