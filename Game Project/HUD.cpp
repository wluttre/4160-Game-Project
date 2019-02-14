//Displays UI elements to the screen
#include "HUD.h"
#include "gamedata.h"
#include "ioMod.h"
  
HUD::HUD(const std::string& n, std::vector<std::string> i): name(n),
  rectangle({Gamedata::getInstance().getXmlInt(name + "/rect/x"),
  Gamedata::getInstance().getXmlInt(name + "/rect/y"),
  Gamedata::getInstance().getXmlInt(name + "/rect/width"),
  Gamedata::getInstance().getXmlInt(name + "/rect/height")}),
  instructions(i)
{}
void HUD::draw(SDL_Renderer* renderer) const
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 127);
  SDL_RenderFillRect(renderer, &rectangle);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
  int i = 0;
  for(const std::string s : instructions)
  {
    IoMod::getInstance().writeText(s, rectangle.x + 1, rectangle.y + 20*i);
    ++i;
  }
}

void HUD::update(std::vector<std::string> i)
{
  instructions = i;
}
