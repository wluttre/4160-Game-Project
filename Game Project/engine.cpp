#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twowaymultisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "collisionStrategy.h"
#include "smartSprite.h"
#include "lightSource.h"
#include "torchSprite.h"
#include "flare.h"

Engine::~Engine() { 
  delete Hud;
  delete Hud2;
  for(auto& it : torches){ 
      delete it;
    }
  for(auto& it : smartSprites){ 
      delete it;
    }
  for(auto& it : tiles){ 
      delete it;
    }
  delete player;
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world2("back2", Gamedata::getInstance().getXmlInt("back2/factor") ),
  world3("back3", Gamedata::getInstance().getXmlInt("back3/factor") ),
  world4("back4", Gamedata::getInstance().getXmlInt("back4/factor") ),
  viewport( Viewport::getInstance() ),
  tiles(),
  torches(),
  player(new Player("Bat", "PlayerLight")),
  smartSprites(),
  strategies(),
  Hud(new HUD("HUD", {"W = Jump", "A = Move Left", "D = Move Right", "LShift = shootFlare(only 5)", "Space = Light Torch"})),
  Hud2(new HUD("HUD2", {"Active Flare List: 0", "Free Flare List: 0", "F2 to toggle this HUD"})),
  collision(false),
  visibleHUD(true),
  visibleHUD2(true),
  hasEnded(false),
  godMode(false),
  currentSprite(0),
  makeVideo( false )
{
  player->setScale(0.5);

  //placing torches
  torches.reserve(Gamedata::getInstance().getXmlInt("Torch/number"));
  for(int i = 0; i < Gamedata::getInstance().getXmlInt("Torch/number"); ++i)
  {
    torches.emplace_back(new TorchSprite("Torch", "TorchLight"));
    torches[i]->setScale(0.3);
  }
    torches[0]->setX(200);
    torches[0]->setY(140);
    torches[1]->setX(350);
    torches[1]->setY(140);
    torches[2]->setX(500);
    torches[2]->setY(140);

    torches[3]->setX(500);
    torches[3]->setY(330);
    torches[4]->setX(350);
    torches[4]->setY(330);
    torches[5]->setX(200);
    torches[5]->setY(330);

    torches[6]->setX(275);
    torches[6]->setY(520);
    torches[7]->setX(500);
    torches[7]->setY(520);
    torches[8]->setX(675);
    torches[8]->setY(520);

    torches[9]->setX(675);
    torches[9]->setY(710);
    torches[10]->setX(100);
    torches[10]->setY(710);

    torches[11]->setX(1450);
    torches[11]->setY(710);
    torches[12]->setX(1450);
    torches[12]->setY(470);

    torches[13]->setX(2310);
    torches[13]->setY(710);
    torches[14]->setX(3100);
    torches[14]->setY(520);
    torches[15]->setX(2400);
    torches[15]->setY(210);
    torches[16]->setX(3100);
    torches[16]->setY(320);
  //placing level geometry
  tiles.reserve(Gamedata::getInstance().getXmlInt("Tile/number"));
  for(int i = 0; i < Gamedata::getInstance().getXmlInt("Tile/number"); ++i)
  {
    tiles.emplace_back(new Sprite("Tile"));
    tiles[i]->setScale(0.3);
    if(i < 5){
      tiles[i]->setX(0);
      tiles[i]->setY(tiles[i]->getScaledHeight()*i);
    }
    else if(i < 25){
      tiles[i]->setX(tiles[i]->getScaledWidth()* (i - 5));
      tiles[i]->setY(0);
    }
    else if(i < 40){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 25));
      tiles[i]->setY(tiles[i]->getScaledHeight() * 5);
    }
    else if(i < 58){
      tiles[i]->setX(tiles[i]->getScaledWidth()*19);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 40));
    }
    else if(i < 76){
      tiles[i]->setX(tiles[i]->getScaledWidth()*20);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 58));
    }
    else if(i < 91){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 76));
      tiles[i]->setY(tiles[i]->getScaledHeight() * 5);
    }
    else if(i < 100){
      tiles[i]->setX(0);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 91) + tiles[i]->getScaledHeight()*5);
    }
    else if(i < 115){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 100) + tiles[i]->getScaledWidth()*5);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 10);
    }
    else if(i < 125){
      tiles[i]->setX(0);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 115) + tiles[i]->getScaledHeight()*10);
    }
    else if(i < 140){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 125) - tiles[i]->getScaledWidth());
      tiles[i]->setY(tiles[i]->getScaledHeight() * 15);
    }
    else if(i < 150){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 140) - tiles[i]->getScaledWidth()*2);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 20);
    }
    else if(i < 160){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 150) - tiles[i]->getScaledWidth()*2);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 21);
    }
    else if(i < 170){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 160) - tiles[i]->getScaledWidth()*2);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 22);
    }
    else if(i < 220){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 170) + tiles[i]->getScaledWidth()*12);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 20);
    }
    else if(i < 270){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 220) + tiles[i]->getScaledWidth()*12);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 21);
    }
    else if(i < 320){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 270) + tiles[i]->getScaledWidth()*12);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 22);
    }
    else if(i < 320){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 270) + tiles[i]->getScaledWidth()*14);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 22);
    }
    else if(i < 335){
      tiles[i]->setX(tiles[i]->getScaledWidth()*33);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 320) + tiles[i]->getScaledHeight()*3);
    }
    else if(i < 350){
      tiles[i]->setX(tiles[i]->getScaledWidth()*34);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 335) + tiles[i]->getScaledHeight()*3);
    }
    else if(i < 359){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 350) + tiles[i]->getScaledWidth()*35);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 15);
    }
    else if(i < 368){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 359) + tiles[i]->getScaledWidth()*35);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 14);
    }
    else if(i < 376){
      tiles[i]->setX(tiles[i]->getScaledWidth()*42);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 368) + tiles[i]->getScaledHeight()*3);
    }
    else if(i < 377){
      tiles[i]->setX(tiles[i]->getScaledWidth()*42);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 376) + tiles[i]->getScaledHeight()*13);
    }
    else if(i < 379){
      tiles[i]->setX(tiles[i]->getScaledWidth()*42);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 377) + tiles[i]->getScaledHeight()*16);
    }
    else if(i < 387){
      tiles[i]->setX(tiles[i]->getScaledWidth()*43);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 379) + tiles[i]->getScaledHeight()*3);
    }
    else if(i < 388){
      tiles[i]->setX(tiles[i]->getScaledWidth()*47);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 387) + tiles[i]->getScaledHeight()*19);
    }
    else if(i < 390){
      tiles[i]->setX(tiles[i]->getScaledWidth()*43);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 388) + tiles[i]->getScaledHeight()*16);
    }
    else if(i < 395){
      tiles[i]->setX(tiles[i]->getScaledWidth()*35);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 390) + tiles[i]->getScaledHeight()*3);
    }
    else if(i < 400){
      tiles[i]->setX(tiles[i]->getScaledWidth()*36);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 395) + tiles[i]->getScaledHeight()*3);
    }
    else if(i < 405){
      tiles[i]->setX(tiles[i]->getScaledWidth()*37);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 400) + tiles[i]->getScaledHeight()*3);
    }
    else if(i < 410){
      tiles[i]->setX(tiles[i]->getScaledWidth()*38);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 405) + tiles[i]->getScaledHeight()*3);
    }
    else if(i < 415){
      tiles[i]->setX(tiles[i]->getScaledWidth()*39);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 410) + tiles[i]->getScaledHeight()*3);
    }
    else if(i < 420){
      tiles[i]->setX(tiles[i]->getScaledWidth()*40);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 415) + tiles[i]->getScaledHeight()*3);
    }
    else if(i < 425){
      tiles[i]->setX(tiles[i]->getScaledWidth()*41);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 420) + tiles[i]->getScaledHeight()*3);
    }
    //broken sections
    else if(i < 427){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 425) + tiles[i]->getScaledWidth()*48);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 19);
    }
    else if(i < 429){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 427) + tiles[i]->getScaledWidth()*48);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 18);
    }
    else if(i < 433){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 429) + tiles[i]->getScaledWidth()*15);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 9);
    }
    else if(i < 437){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 433) + tiles[i]->getScaledWidth());
      tiles[i]->setY(tiles[i]->getScaledHeight() * 14);
    }
    else if(i < 439){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 437) + tiles[i]->getScaledWidth()*17);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 15);
    }
    else if(i < 442){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 439) + tiles[i]->getScaledWidth()*14);
      tiles[i]->setY(tiles[i]->getScaledHeight() * 19);
    }
    //last third
    else if(i < 460){
      tiles[i]->setX(tiles[i]->getScaledWidth()*58);
      tiles[i]->setY(tiles[i]->getScaledHeight() * (i - 442));
    }
    else if(i < 478){
      tiles[i]->setX(tiles[i]->getScaledWidth()*59);
      tiles[i]->setY(tiles[i]->getScaledHeight() * (i - 460));
    }
    else if(i < 483){
      tiles[i]->setX(tiles[i]->getScaledWidth()*65);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 478) + tiles[i]->getScaledHeight()*18);
    }
    else if(i < 488){
      tiles[i]->setX(tiles[i]->getScaledWidth()*66);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 483) + tiles[i]->getScaledHeight()*18);
    }
    else if(i < 493){
      tiles[i]->setX(tiles[i]->getScaledWidth()*67);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 488) + tiles[i]->getScaledHeight()*18);
    }
    else if(i < 498){
      tiles[i]->setX(tiles[i]->getScaledWidth()*77);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 493) + tiles[i]->getScaledHeight()*18);
    }
    else if(i < 503){
      tiles[i]->setX(tiles[i]->getScaledWidth()*78);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 498) + tiles[i]->getScaledHeight()*18);
    }
    else if(i < 508){
      tiles[i]->setX(tiles[i]->getScaledWidth()*79);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 503) + tiles[i]->getScaledHeight()*18);
    }
    else if(i < 516){
      tiles[i]->setX(tiles[i]->getScaledWidth()*83);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 508));
    }
    else if(i < 530){
      tiles[i]->setX(tiles[i]->getScaledWidth()*83);
      tiles[i]->setY(tiles[i]->getScaledHeight()*(i - 516) + tiles[i]->getScaledHeight()*10);
    }
    else if(i < 535){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 530) + tiles[i]->getScaledHeight()*81);
      tiles[i]->setY(tiles[i]->getScaledHeight()*10);
    }

    else if(i < 558){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 530) + tiles[i]->getScaledHeight()*55);
      tiles[i]->setY(0);
    }
    else if(i < 561){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 558) + tiles[i]->getScaledWidth()*75);
      tiles[i]->setY(tiles[i]->getScaledHeight()*11);
    }
    else if(i < 563){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 561) + tiles[i]->getScaledWidth()*81);
      tiles[i]->setY(tiles[i]->getScaledHeight()*15);
    }
    else if(i < 566){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 563) + tiles[i]->getScaledWidth()*75);
      tiles[i]->setY(tiles[i]->getScaledHeight()*6);
    }
    else if(i < 574){
      tiles[i]->setX(tiles[i]->getScaledWidth()*(i - 566) + tiles[i]->getScaledWidth()*60);
      tiles[i]->setY(tiles[i]->getScaledHeight()*7);
    }
  }

  /*smartSprites.reserve(Gamedata::getInstance().getXmlInt("Bat/number"));
  for(int i = 0; i < Gamedata::getInstance().getXmlInt("Bat/number"); ++i)
  {
    smartSprites.emplace_back(new SmartSprite("Bat", player->getPlayer()->getPosition(), 
                                                     player->getPlayer()->getScaledWidth(), 
                                                     player->getPlayer()->getScaledHeight()));
    player->attach(smartSprites[i]);
  }*/

  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player->getPlayer());
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  world.draw();
  world2.draw();
  world3.draw();
  world4.draw();

  for(const TorchSprite* s : torches)
    s->draw(renderer);
  player->draw(renderer);
  for(const Drawable* s : tiles)
    s->draw();

  for(const SmartSprite* s : smartSprites)
    s->draw();

  //faux darkness
  std::vector<const LightSource*> lights;
  lights.push_back(player->getLight());
  for(const TorchSprite* s : torches)
    lights.push_back(s->getLight());
  std::list<Flare*> fs = player->getFlares();
  for(Flare* f : fs)
    lights.push_back(f->getLight());

  if(visibleHUD)
     Hud->draw(renderer);
  if(visibleHUD2)
     Hud2->draw(renderer);


  viewport.draw();
  if(hasEnded)
  {
     if(!player->getLight()->isAlive())
        {
          io.writeText("Game Over \n Press R to try again", 300, 250, SDL_Color({255, 0, 0, 0}));
        }
        else
        {
          io.writeText("Congratulations, You Escaped! \n Press R to play again", 200, 250, SDL_Color({255, 255, 255, 0}));
        }
  }
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  if(player->getY() > Gamedata::getInstance().getXmlInt("world/height"))
  {
    player->getLight()->takeDamage(100.0);
  }
  if(!hasEnded)
  {
    if(player->getLight()->isAlive() && !godMode)
      player->getLight()->takeDamage(0.3);
    player->update(ticks);
    checkForCollisions();
    for(TorchSprite* s : torches)
      s->update(ticks);
    for(Drawable* s : tiles)
      s->update(ticks);
    for(SmartSprite* s : smartSprites)
      s->update(ticks);
  }
  if(player->getX() > Gamedata::getInstance().getXmlInt("world/width"))
  {
    hasEnded = true;
  }
  if(!player->getLight()->isAlive())
  {
    hasEnded = true;
  }
  std::stringstream strm;
  strm <<player->getActiveFlares();
  std::string a = strm.str();
  std::stringstream strm2;
  strm2 <<player->getFreeFlares();
  std::string f = strm2.str();
  Hud2->update({"Active Flare List: " + a, "Free Flare List: " + f, "F2 to toggle this HUD"});

  world.update();
  world2.update();
  world3.update();
  world4.update();
  viewport.update(); // always update viewport last
}

void Engine::checkForCollisions() {
  for(Drawable* s : tiles) {
    if ( strategies[0]->execute(*(player->getPlayer()), *s) ) {
        player->floorCollide(s);
    }
  }
  std::list<Flare*> fs = player->getFlares();
  for(Flare* f : fs){
    for(Drawable* s : tiles){
      if( strategies[0]->execute(*f->getSprite(), *s))
        f ->floorCollide(s);
    }
  }

  for(SmartSprite* s : smartSprites){
      if(strategies[0]->execute(s->getSprite(), *(player->getPlayer()))){
        s->getSprite().explode();
      }
  }
}

/*void Engine::switchSprite(){
  ++currentSprite;
  if(currentSprite == 0)
  {
    Viewport::getInstance().setObjectToTrack(player->getPlayer());
  }
  else
  {
    currentSprite = currentSprite % (sprites.size());
    Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
  }
}*/

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        if ( keystate[SDL_SCANCODE_G] ) {
          godMode = !godMode;
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          //switchSprite();
        }
        if (keystate[SDL_SCANCODE_F1] ) {
          visibleHUD = !visibleHUD;
        }
        if (keystate[SDL_SCANCODE_F2] ) {
          visibleHUD2 = !visibleHUD2;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      //if (event.type == SDL_MOUSEBUTTONDOWN) {
        //if (event.button.button == SDL_BUTTON_LEFT){
      if (keystate[SDL_SCANCODE_SPACE]) {   
          for(unsigned int i = 0; i < torches.size(); ++i){ 
            if(strategies[0]->execute(*(player->getPlayer()), 
                                        *(torches[i]->getSprite())))
            player->interact(*torches[i]);
          }
        //}
      }
      if (keystate[SDL_SCANCODE_LSHIFT]) {   
          player->shoot();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
