//header file for the torchsprite class, which creates the torch object in the game
#ifndef TORCHSPRITE__H
#define TORCHSPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "multisprite.h"
#include "lightSource.h"

class TorchSprite {
public:
  TorchSprite(const std::string name, const std::string lightName);
  TorchSprite(const TorchSprite&);

  void draw(SDL_Renderer* r) const;
  void update(Uint32 ticks);

  
  const LightSource* getLight() const {return &torchLight;}
  LightSource* getLight() {return &torchLight;}
  const MultiSprite* getSprite() const {return images;}
  void setScale(float x){
    on.setScale(x);
    off.setScale(x);
    }
void setX(float x){
    on.setX(x);
    off.setX(x);
    }
void setY(float y){
    on.setY(y);
    off.setY(y);
    }


private:
  
  MultiSprite on, off;
  MultiSprite* images;
  LightSource torchLight;

  TorchSprite& operator=(const TorchSprite&);

};
#endif
