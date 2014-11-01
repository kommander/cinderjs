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
#ifndef _StaticFactory_hpp_
#define _StaticFactory_hpp_

#pragma once

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

#include "cinder/gl/Material.h"
#include "cinder/gl/Light.h"
#include "cinder/Ray.h"
#include "cinder/Camera.h"

#include "v8.h"

//
// The factory manages the references between C and js.
// Modules request objects from the factory and the factory takes care of cleanup

// TODO: store per isolate/context (removable when module is unloaded)

namespace cjs {
  
  struct FactorySettings {
    uint16_t maxObjects = 4096;
  };
  
  struct FactoryStats {
    int puts;
    int removes;
  };
  
  class StaticFactory {
    // TODO: (GC loop (with Timer)) GC finish callback to check for orphaned handles and remove its wraps
    
    //
    // FIXME: Leaking memory
    // FIXME: Not thread safe
    template<class T>
    class Wrapper {
      public:
      uint32_t id;
      boost::shared_ptr<T> value;
      
      ~Wrapper(){
        if(!handle_.IsEmpty() && handle_.IsWeak()){
          handle_.ClearWeak();
          handle_.Reset();
        }
        value.reset();
      }
      
      inline v8::Persistent<v8::Object>& persistent() {
        return handle_;
      }
      
      inline void Wrap(v8::Handle<v8::Object> handle) {
        assert(persistent().IsEmpty());
        assert(handle->InternalFieldCount() > 0);
        handle->SetAlignedPointerInInternalField(0, this);
        handle_.Reset(v8::Isolate::GetCurrent(), handle);
        MakeWeak();
      }
      
      private:
      inline void MakeWeak(void) {
        persistent().SetWeak(this, WeakCallback);
        persistent().MarkIndependent();
      }
      
      template<class TT>
      static void WeakCallback(const v8::WeakCallbackData<v8::Object, Wrapper<TT>>& data) {
        v8::Isolate* isolate = data.GetIsolate();
        v8::HandleScope scope(isolate);
        Wrapper<TT>* wrap = data.GetParameter();
        assert(wrap->handle_.IsNearDeath());
        assert(data.GetValue() == v8::Local<v8::Object>::New(isolate, wrap->handle_));
        wrap->handle_.Reset();
        StaticFactory::remove<TT>( isolate, wrap->id );
        //delete wrap;
      }

      v8::Persistent<v8::Object> handle_;
    };
    
    public:
    
      static void initialize(){
        
      };
    
      template<class T>
      static v8::Handle<v8::Object> create( v8::Isolate* isolate ){
        return put( isolate, boost::shared_ptr<T>(new T()) );
      }
    
      template<class T>
      static v8::Handle<v8::Object> put( v8::Isolate* isolate, boost::shared_ptr<T> value ){
        v8::EscapableHandleScope scope(isolate);
        
        boost::shared_ptr<Wrapper<T>> tuple( new Wrapper<T>() );
        
        // TODO: Replace with _getId()
        tuple->id = ++_sObjectCounter;
        tuple->value = value;
        
        v8::Local<v8::ObjectTemplate> obj = v8::ObjectTemplate::New();
        obj->SetInternalFieldCount(1);
        
        v8::Local<v8::Object> idHolder = obj->NewInstance();
        idHolder->Set(v8::String::NewFromUtf8(isolate, "id"), v8::Uint32::New(isolate, tuple->id));
        
        tuple->Wrap(idHolder);
        
        // Store
        std::pair<uint32_t, boost::any> pair(tuple->id, tuple);
        if(_sObjectMap.insert(pair).second == false){
          std::cout << "Could not insert into _sObjectMap." << std::endl;
          pair.second.clear();
          tuple.reset();
          v8::Local<v8::Boolean> undef = v8::Boolean::New(isolate, false);
          return scope.Escape( undef.As<v8::Object>() );
        }
        
        // stats
        _stats.puts++;
        
        // Todo: check if needed
        isolate->AdjustAmountOfExternalAllocatedMemory(sizeof(*tuple));
        
        return scope.Escape(idHolder);
      }
    
      template<class T>
      static boost::shared_ptr<T> get( uint32_t id ){
        boost::any wrap = _sObjectMap[id];
        if(wrap.empty()){
          return boost::shared_ptr<T>();
        } else {
          // If conversion fails, the ids are probably mixed up
          // and the map returns the wrong object so it cannot be converted to T
          boost::shared_ptr<Wrapper<T>> proxy = boost::any_cast<boost::shared_ptr<Wrapper<T>>>(wrap);
          return proxy->value;
        }
      }
    
      template<class T>
      static void remove( v8::Isolate* isolate, uint32_t id ){
        boost::any wrap = _sObjectMap[id];
        if(!wrap.empty()){
          boost::shared_ptr<Wrapper<T>> proxy = boost::any_cast<boost::shared_ptr<Wrapper<T>>>(wrap);
          isolate->AdjustAmountOfExternalAllocatedMemory(-sizeof(*proxy));
          wrap.clear();
          _sObjectMap.erase(id);
          proxy.reset();
        }
        // Todo: release id for re-use
        
        // stats
        _stats.removes++;
        
      }
    
      static unsigned long size(){
        return _sObjectMap.size();
      }
    
      static FactoryStats& getStats(){
        return _stats;
      }
    
    private:
      // TODO: Improve counters and reuse unused ids (generate ids at startup)
      //       -> If id space is limited, it would help to spot leaks in JS scripts
      static std::map<uint32_t, boost::any> _sObjectMap;
      static uint32_t _sObjectCounter;
      static FactorySettings _settings;
      static FactoryStats _stats;
  };
  
} // namespace

#endif