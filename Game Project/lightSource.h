#ifndef LIGHTSOURCE__H
#define LIGHTSOURCE__H
#include <SDL.h>
#include "drawable.h"
#include "light.h"



class LightSource {
public:
  LightSource(const std::string& name, Drawable* s);

  void draw(SDL_Renderer* renderer) const;
  void update();
  const float& getRadius() const {return radius;}
  const float& getCenterX() const {return x;}
  const float& getCenterY() const {return y;}
  void takeDamage(float damage);
  void Refresh(float value);
  bool isAlive(){return hp;}


private:
  const float startingRadius;
  float hp;
  float radius;
  Drawable* sprite;
  float x;
  float y;

  //light?
};
#endif
