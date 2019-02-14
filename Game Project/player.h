//header file of the player class, which creates the player object of the game
#ifndef PLAYER__H
#define PLAYER__H
#include <SDL.h>
#include <string>
#include <list>
#include <vector>
#include <cmath>

#include "twowaymultisprite.h"
#include "lightSource.h"
#include "torchSprite.h"
#include "flare.h"

class SmartSprite;

class Player {
public:
  Player(const std::string&, const std::string&);
  Player(const Player&) = delete;
  ~Player();

  void draw(SDL_Renderer* r) const;
  void update(Uint32 ticks);
  const TwoWayMultiSprite* getPlayer() const { return player; }

  const std::string& getName() const { return player->getName(); }
  int getX() const { return player->getX(); }
  int getY() const { return player->getY(); }
  const Image* getImage() const { 
    return player->getImage();
  }
  int getScaledWidth()  const { 
    return player->getScaledWidth();
  } 
  int getScaledHeight()  const { 
    return player->getScaledHeight();
  } 
  const SDL_Surface* getSurface() const { 
    return player->getSurface();
  }

  void setScale(float x){player->setScale(x);}

  void right();
  void left();
  void up();
  void down();
  void interact(TorchSprite&);
  void shoot();
  void stop();
  void floorCollide(Drawable*);

  void attach( SmartSprite* o ) { observers.push_back(o); } 
  void detach( SmartSprite* o );
  
  const LightSource* getLight() const {return &playerLight;}
  LightSource* getLight() {return &playerLight;}
  const std::list<Flare*> getFlares() const {return flareList;}
  int getActiveFlares() const {return flareList.size();}
  int getFreeFlares() const {return freeList.size();}

  Player& operator=(const Player&) = delete;

private:
  TwoWayMultiSprite* player;
  Vector2f initialVelocity;
  LightSource playerLight;

  std::list<SmartSprite*> observers;

  std::list<Flare*> flareList;
  std::list<Flare*> freeList;
  float flareSpeed;
  float flareInterval;
  float timeSinceLastFlare;
  int bulletCount;

  bool inAir;
  bool facingRight;
  int gravity;

  int worldWidth;
  int worldHeight;
};
#endif
