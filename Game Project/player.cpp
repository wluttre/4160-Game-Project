//class that implements the player object that users can control while the game is running
#include "player.h"
#include "smartSprite.h"
#include "gamedata.h"

Player::Player( const std::string& name, const std::string& lightName) :
  player(new TwoWayMultiSprite(name)),
  initialVelocity(player->getVelocity()),
  playerLight(lightName, player),
  observers(),
  flareList(),
  freeList(),
  flareSpeed(Gamedata::getInstance().getXmlInt("Flare/speed")),
  flareInterval(Gamedata::getInstance().getXmlInt("Flare/interval")),
  timeSinceLastFlare(0),
  bulletCount(5),
  inAir(true),
  facingRight(true),
  gravity(Gamedata::getInstance().getXmlInt("world/gravity")),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Player::~Player(){
  for(auto& it : flareList){ 
      delete it;
    }
  for(auto& it : freeList){ 
      delete it;
    }
  delete player;

}

void Player::stop() { 
  player->setVelocity( Vector2f(0, 0) );
}

void Player::right() { 
  //if ( player->getX() < worldWidth-getScaledWidth()) {
    player->setVelocityX(250);
  //}
  facingRight = true;
} 
void Player::left()  { 
  //if ( player->getX() > 0) {
    player->setVelocityX(-250);
  //}
  facingRight = false;
} 
void Player::up()    { 
  if(!inAir){
    inAir = true;
    //if ( player->getY() > 0) {-425
      player->setVelocityY(-425);
    //}
  }
} 
void Player::down()  { 
  /*if(playerLight.isAlive())
    playerLight.takeDamage(0.1);
  else
    player->explode();*/
}

void Player::interact(TorchSprite& t)  { 
  if(&t != NULL)
  {
    t.getLight()->Refresh(100);
    playerLight.Refresh(100);
  }
}

void Player::shoot(){
  if(timeSinceLastFlare < flareInterval) return;
  //std::cout<<"Active List: " << flareList.size()<< ", Free List: " << freeList.size() <<std::endl;
  if(bulletCount > 0)
  {
    if(freeList.empty())
    {
      //std::cout<<"trying to shoot" <<std::endl;
      Flare* flare = new Flare("Flare", "FlareLight");
      flare->setScale(0.5);

      ///if facing right

      flare->getSprite()->setPosition( player->getPosition());
      if(facingRight)
        flare->getSprite()->setVelocity( Vector2f(flareSpeed + player->getVelocityX(), -100) );
      else
        flare->getSprite()->setVelocity( -1*Vector2f(flareSpeed - player->getVelocityX(), 100) );
      flareList.push_back(flare);
    }
    else
    {
      auto it = freeList.begin();
      (*it)->reset();
      (*it)->getSprite()->setPosition( player->getPosition());
      if(facingRight)
        (*it)->getSprite()->setVelocity( Vector2f(flareSpeed + player->getVelocityX(), -300) );
      else
        (*it)->getSprite()->setVelocity( -1*Vector2f(flareSpeed - player->getVelocityX(), 300) );
      flareList.push_back(*it);
      it = freeList.erase(it);
    }
    timeSinceLastFlare = 0;
    bulletCount--;
  }
}

void Player::draw(SDL_Renderer* r) const { 
  player->draw();
  playerLight.draw(r);
  for ( const Flare* flare : flareList ) {
    flare->draw(r);
  }

}

void Player::floorCollide(Drawable* floor)
{
  float rightOverlap = player->getX() + player->getScaledWidth() - floor->getX();
  float leftOverlap = floor->getX() + floor->getScaledWidth() - player->getX();
  float topOverlap = player->getY() + player->getScaledHeight() - floor->getY();
  float bottomOverlap = floor->getY() + floor->getScaledHeight() - player->getY();
  //std::cout<<"left overlap = " <<leftOverlap << "right overlap = " <<rightOverlap<<std::endl;
  //std::cout<<"top overlap = " <<topOverlap << "bottom overlap = " <<bottomOverlap<<std::endl;
  float xOverlap, yOverlap;

  if(rightOverlap < leftOverlap)
     xOverlap = -rightOverlap;
  else
     xOverlap = leftOverlap;

  if(topOverlap < bottomOverlap){
     yOverlap = -topOverlap;
     if(topOverlap < abs(player->getVelocityY())/2 && 
        !(player->getX() + player->getScaledWidth() <= floor->getX() + 7 ||
        player->getX() + 7 >= floor->getX() + floor->getScaledWidth() ) ) {
       inAir = false;
       if(player->getVelocityY() > 0)
         player->setVelocityY(0);
     }
  }
  else {
    yOverlap = bottomOverlap;
    if(bottomOverlap < abs(player->getVelocityY())/2 &&
       player->getVelocityY() < 0 &&
       !(player->getX() + player->getScaledWidth() <= floor->getX() + 7||
       player->getX() + 7 >= floor->getX() + floor->getScaledWidth() ) )
      player->setVelocityY(50);
  }
  if(abs(xOverlap) < abs(yOverlap))
    player->setX(player->getX() + xOverlap);
  else if(player->getVelocityY()  < 0)
  {
    player->setX(player->getX() + xOverlap);
  }
  else
    player->setY(player->getY() + yOverlap);
}

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::update(Uint32 ticks) {
  if(!playerLight.isAlive())
    player->explode();
  timeSinceLastFlare += ticks;
  auto it = flareList.begin();
  while ( it != flareList.end()) {
    if((*it)->getLight()->isAlive()){
      (*it)->update(ticks);
      ++it;
    }
    else{
      freeList.push_back(*it);
      it = flareList.erase(it);
    }
      
  }
  player->update(ticks);
  playerLight.update();
  player->setVelocityX(0);
  if(inAir && player->getVelocityY() <= Gamedata::getInstance().getXmlInt(player->getName() + "/speedY") * 5)
  {
    player->setVelocityY(player->getVelocityY() + gravity);
  }
  else
    player->setVelocityY(0);
  inAir = true;
  

  ///getting rid of this later
  /*std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( player->getPosition() );
    ++ptr;
  }*/
  //stop();
}
