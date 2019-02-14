//header file of the world class, which contains the overall game world and the viewport
#include <string>
#include "image.h"
#include "viewport.h"

class World {
public:
  World(const std::string& name, int fact);
  void update();
  void draw() const;
private:
  Image* const image;
  int factor;
  unsigned worldWidth;
  unsigned imageWidth;
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
  World& operator=(const World&);
};
