#include "lightSource.h"
#include "gamedata.h"
#include "viewport.h"

LightSource::LightSource( const std::string& name, Drawable* s) :
  startingRadius(Gamedata::getInstance().getXmlFloat(name + "/radius")),
  hp(Gamedata::getInstance().getXmlFloat(name + "/hp")),
  radius(startingRadius * hp/100.0),
  sprite(s),
  x(sprite->getX() + sprite->getScaledWidth()/2),
  y(sprite->getY() + sprite->getScaledHeight()/2)
{}

void LightSource::draw(SDL_Renderer* renderer) const{
 SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
     int step = 360/360;
  for (int theta = 0; theta < 360; theta += step) {
    SDL_RenderDrawPoint(renderer, 
      x+radius*cos(((3.1415926535897/180) * theta)) - Viewport::getInstance().getX(), 
      y+radius*sin(((3.1415926535897/180) * theta)) - Viewport::getInstance().getY()
    );
  }
//draw a circle for testing
}

void LightSource::update() {
  x = sprite->getPosition()[0] + sprite->getScaledWidth()/2;
  y = sprite->getPosition()[1] + sprite->getScaledHeight()/2;
  radius = startingRadius * hp/100.0;
}

void LightSource::takeDamage(float damage){
  hp -= damage;
  if(hp < 0)
    hp = 0;
}

void LightSource::Refresh(float value){
  hp += value;
  if(hp > 100)
    hp = 100;
}
