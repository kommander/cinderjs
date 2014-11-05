//
//  Console.cpp
//  cinderjs
//
//  Created by Sebastian Herrlinger on 16/10/14.
//
//

#include <stdio.h>
#include "AppConsole.h"

namespace cjs {
  
bool AppConsole::sInitialized = false;
bool AppConsole::sChanged = false;
int AppConsole::sNumLinesToShow = 10;
int AppConsole::sMaxLines = 100;
cinder::gl::TextureRef AppConsole::_sTexture;
std::vector<std::string> AppConsole::sLines = std::vector<std::string>();
int AppConsole::linesToShow = 20;

}