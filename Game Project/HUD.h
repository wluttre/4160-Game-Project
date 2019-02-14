//Header file of the class implementing UI elements
#ifndef HUD__H
#define HUD__H

#include <SDL.h>
#include <vector>
#include "drawable.h"


class HUD {
public:
  HUD(const std::string&, std::vector<std::string>);
  void draw(SDL_Renderer*) const;
  void update(std::vector<std::string>);

private:
  std::string name;
  SDL_Rect rectangle;
  std::vector<std::string> instructions;

};
#endif
