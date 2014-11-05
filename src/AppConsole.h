//
//  Console.h
//  cinderjs
//
//  Created by Sebastian Herrlinger on 16/10/14.
//
//

#ifndef cinderjs_Console_h
#define cinderjs_Console_h

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Color.h"

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>

/**
 * AppConsole
 * Provides a visual logging interface component for the GL render context
 * TODO: Optimize, currently very energy hungry due to the constant texture creation
 */

namespace cjs {

class AppConsole {
  public:
    inline static void initialize(){
      
    }
  
    inline static void log( std::string str ) {
      sLines.push_back( str );
      
      if(sLines.size() > 100){
        // TODO: Remove previous lines from buffer
      }
      sChanged = true;
    }
    
    // Draw
    inline static void draw( cinder::vec2 &pos ){
      
      if( sChanged ) {
        sChanged = false;
        
        cinder::TextLayout text;
        text.setColor( cinder::ColorA( 1, 1, 1, 1 ) );
        
        int end = sLines.size() > linesToShow ? linesToShow : sLines.size();
        
        for(std::vector<std::string>::iterator it = sLines.end() - end; it != sLines.end(); ++it) {
          text.addLine(*it);
        }
        
        _sTexture = cinder::gl::Texture::create( text.render() );
      }
      
      cinder::gl::pushMatrices();
      cinder::gl::translate(2, pos.y - linesToShow * 12, 0);
      cinder::gl::draw( _sTexture );
      cinder::gl::popMatrices();
    }
    
  private:
    static bool sInitialized;
    static bool sChanged;
    static std::vector<std::string> sLines;
    static int sNumLinesToShow;
    static int sMaxLines;
    static cinder::gl::TextureRef _sTexture;
    static int linesToShow;
};
  
} // namespace cjs

#endif
