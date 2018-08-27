#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "twowaymultisprite.h"

class SmartSprite{
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&);
  virtual ~SmartSprite() { } 

  void draw() const { sprite.draw(); }

  virtual void update(Uint32 ticks);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  TwoWayMultiSprite& getSprite() { return sprite; }

private:
  enum MODE {NORMAL, EVADE};
  TwoWayMultiSprite sprite;
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif
