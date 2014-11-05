//
//  CinderAppBase.cpp
//  cinderjs
//
//  Created by Sebastian Herrlinger on 04/11/14.
//
//

#include "CinderAppBase.hpp"

namespace cjs {
  
  std::map<std::string, std::shared_ptr<PipeModule>> CinderAppBase::NAMED_MODULES;

}