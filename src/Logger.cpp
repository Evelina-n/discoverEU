/*
   Copyright 2020 Blanovschi Dinu

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   */

#include <Logger.hpp>
#include <iostream>

DEU::Logger *DEU::Logger::s_Logger;

DEU::Logger::Logger(){
  fl = &std::cerr;
  s_Logger = this;
}
DEU::Logger::Logger(std::string log_file) {
  fl = new std::ofstream(log_file, std::ios::out | std::ios::app);
  s_Logger = this;
}

void DEU::Logger::log(DEU::LogLevel level, std::string message) {
#ifdef NDEBUG
  if(level <= DEBUG)return;
#endif
  *fl << message << std::endl;
}
