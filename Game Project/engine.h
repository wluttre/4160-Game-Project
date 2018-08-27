#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "HUD.h"

//player is also subject sprite
class Player;
class CollisionStrategy;
class SmartSprite;
class TorchSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  //void switchSprite();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World world;
  World world2;
  World world3;
  World world4;
  Viewport& viewport;

  std::vector<Drawable*> tiles;
  std::vector<TorchSprite*> torches;
  Player* player;
  std::vector<SmartSprite*> smartSprites;
  std::vector<CollisionStrategy*> strategies;
  HUD* Hud;
  HUD* Hud2;

  bool collision;
  bool visibleHUD, visibleHUD2;

  bool hasEnded;
  bool godMode;


  int currentSprite;

  bool makeVideo;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
