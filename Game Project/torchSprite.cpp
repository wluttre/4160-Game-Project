//class that implements the torch object in the game, which the player can interact with.
#include "torchSprite.h"
#include "gamedata.h"

TorchSprite::TorchSprite(const std::string name, const std::string lightName):
  on(name + "/on"),
  off(name + "/off"),
  images(&off),
  torchLight(lightName, images)
{
  ///won't need later
  on.setPosition(off.getPosition());
}

void TorchSprite::draw(SDL_Renderer* r) const
{
  images->draw();
  torchLight.draw(r);
}

void TorchSprite::update(Uint32 ticks)
{
  if(torchLight.isAlive())
    images = &on;
  else
    images = &off;
  images->update(ticks);
  torchLight.update();
}
