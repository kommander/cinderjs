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

namespace cjs {

class AppConsole {
  public:
    inline static void initialize(){
      
    }
  
    inline static void log( std::string str ) {
      sLines.push_back( str );
      sChanged = true;
    }
    
    // Draw
    inline static void draw( cinder::Vec2f &pos ){
      
      if( sChanged ) {
        sChanged = false;
      }
      
      cinder::gl::pushMatrices();
      
      // FPS
      cinder::TextLayout fpsText;
      fpsText.setColor( cinder::ColorA( 1, 1, 1, 1 ) );
      fpsText.addRightLine( "FPS: " + std::to_string( cinder::app::getFrameRate() ) );
      cinder::gl::draw( cinder::gl::Texture( fpsText.render() ) );
      
      cinder::TextLayout text;
      text.setColor( cinder::ColorA( 1, 1, 1, 1 ) );
      
      for(std::vector<std::string>::iterator it = sLines.begin(); it != sLines.end(); ++it) {
        text.addLine(*it);
        pos.y -= 15;
      }
    
      cinder::gl::translate(2, pos.y - 100, 0);
      cinder::gl::draw( cinder::gl::Texture( cinder::gl::Texture( text.render() ) ) );
      
      
      cinder::gl::popMatrices();
    }
    
  private:
    static bool sInitialized;
    static bool sChanged;
    static std::vector<std::string> sLines;
    static int sNumLinesToShow;
    static int sMaxLines;
};
  
} // namespace cjs

#endif
