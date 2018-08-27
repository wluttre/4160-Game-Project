#include <iostream>
#include <cmath>
#include "flare.h"
#include "gamedata.h"

Flare::Flare(const string name, const string lightName) :
          flare(new MultiSprite(name)),
          flareLight(lightName, flare),
          inAir(true),
          gravity(Gamedata::getInstance().getXmlInt("world/gravity"))
  {
    std::cout<<"BulletCreated!"<<std::endl; 
  }
void Flare::update(Uint32 ticks) {
  flare->setVelocityY(flare->getVelocityY() + gravity);
  flare->update(ticks);

  flareLight.takeDamage(0.5);
  flareLight.update();
}

void Flare::draw(SDL_Renderer* r) const{
  flare->draw();
  flareLight.draw(r);

}

void Flare::floorCollide(Drawable* floor)
{
  float rightOverlap = flare->getX() + flare->getScaledWidth() - floor->getX();
  float leftOverlap = floor->getX() + floor->getScaledWidth() - flare->getX();
  float topOverlap = flare->getY() + flare->getScaledHeight() - floor->getY();
  float bottomOverlap = floor->getY() + floor->getScaledHeight() - flare->getY();
  float xOverlap, yOverlap;

  /*if(rightOverlap < leftOverlap)
     xOverlap = -rightOverlap;
  else
     xOverlap = leftOverlap;

  if(topOverlap < bottomOverlap){
     yOverlap = -topOverlap;
     if(topOverlap < 1){
       inAir = false;
       flare->setVelocityX(0);
       flare->setVelocityY(0);
     }
  }
  else {
    yOverlap = bottomOverlap;
  }
  if(inAir && abs(xOverlap) < abs(yOverlap)) {
    flare->setX(flare->getX() + xOverlap);
    flare->setVelocityX(-flare->getVelocityX());
  }
  else
    flare->setY(flare->getY() + yOverlap);*/
  if(rightOverlap < leftOverlap)
     xOverlap = -rightOverlap;
  else
     xOverlap = leftOverlap;

  if(topOverlap < bottomOverlap){
     yOverlap = -topOverlap;
     if(topOverlap < abs(flare->getVelocityY())/2 && 
        !(flare->getX() + flare->getScaledWidth() <= floor->getX() + 20 ||
        flare->getX() + 20 >= floor->getX() + floor->getScaledWidth() ) ) {
       inAir = false;
       flare->setVelocityX(0);
       flare->setVelocityY(0);
     }
  }
  else {
    yOverlap = bottomOverlap;
    if(bottomOverlap < abs(flare->getVelocityY())/2 &&
       flare->getVelocityY() < 0)
      flare->setVelocityY(100);
  }
  if(abs(xOverlap) <= abs(yOverlap)) {
    flare->setX(flare->getX() + xOverlap);
    flare->setVelocityX(-flare->getVelocityX());
  }
  else
    flare->setY(flare->getY() + yOverlap);

}

