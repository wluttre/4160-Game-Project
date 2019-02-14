// William Luttrell        Data-Driven Object oriented Game Construction
//main file that runs the game engine class
#include "engine.h"

RenderContext* RenderContext::instance = NULL;

int main(int, char*[]) {
   bool keepPlaying = true;
   try {
      while ( keepPlaying ) {
       Engine* engine = new Engine;
       keepPlaying = engine->play();
       delete engine;
     }
      delete RenderContext::getInstance();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
