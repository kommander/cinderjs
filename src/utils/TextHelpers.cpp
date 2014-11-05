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
 
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Vector.h"

using namespace std;
using namespace cinder;

namespace cjs {
  
  //
  // TODO: Make scene objects scriptable by default,
  //       by being able to set a JS draw method for each
  //       (needs investigation and performance check)
  // TODO: Check if implementation is possible where multiple isntances
  //       of the same object can be processed from js, not needing to call
  //       the draw callback for each object separately from C land
  class SceneObject {
    public:
      SceneObject(){}
      ~SceneObject(){}
    
      inline void setPosition( vec3 newPos ){
        pos = newPos;
      }
    
    protected:
      vec3 pos;
      bool active = true;
    
      virtual void draw() = 0;
  };
  
  class SimpleText : public SceneObject {
    public:
      SimpleText(){}
      ~SimpleText(){}
    
      inline void setText( std::string s ) {
        _text = s;
        
        // Prepare surface for rendering
        TextLayout tl;
        tl.setColor( ColorA( 1, 1, 1, 1 ) );
        tl.addLine( _text );
        _texture = gl::Texture::create( tl.render() );
      }
    
      inline void draw() {
        gl::pushMatrices();
        gl::translate( pos );
        gl::draw( _texture );
        gl::popMatrices();
      }
    
    private:
      std::string _text;
      gl::Texture2dRef _texture;

  };
  
} // namespace cinderpipe
