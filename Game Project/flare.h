#ifndef FLARE__H
#define FLARE__H
#include <iostream>
#include "multisprite.h"
#include "lightSource.h"
#include "gamedata.h"

class Flare {
public:
  Flare(const string name, const string lightName);
  Flare(const Flare& f);
  ~Flare() { delete flare;}
  void update(Uint32 ticks);
  void draw(SDL_Renderer* r) const;
  void reset() {
    flareLight.Refresh(100);
    inAir = true;
  }

  void setScale(float x){flare->setScale(x);}

  const LightSource* getLight() const {return &flareLight;}
  LightSource* getLight() {return &flareLight;}
  void floorCollide(Drawable*);

  //const MultiSprite getSprite() const {return flare;}
  MultiSprite* getSprite() {return flare;}
  Flare& operator= (const Flare&);

private:
  MultiSprite* flare;
  LightSource flareLight;
  bool inAir;
  int gravity;
};
#endif
