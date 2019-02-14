//class that creates a sprite that cycles through multiple images, creating an animation
#include "multisprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Vector2f MultiSprite::makeVelocity(int vx, int vy) const {
  if(vx && vy){
    float newvx = Gamedata::getInstance().getRandFloat(vx - Gamedata::getInstance().getRandInRange(0, 50), vx + Gamedata::getInstance().getRandInRange(0, 50));
    float newvy = Gamedata::getInstance().getRandFloat(vy - Gamedata::getInstance().getRandInRange(0, 50), vy + Gamedata::getInstance().getRandInRange(0, 50));
    newvx *= [](){ if(rand()%2) return -1; else return 1; }();
    newvy *= [](){ if(rand()%2) return -1; else return 1; }();

    return Vector2f(newvx, newvy);
  }
    return Vector2f(vx, vy);
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY")))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  images(s.images),
  explosion(nullptr),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

MultiSprite::~MultiSprite( ) { if (explosion) delete explosion; }


MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  explosion = nullptr;
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void MultiSprite::explode() {
  if ( !explosion ) {
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

void MultiSprite::draw() const { 
  if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
}

void MultiSprite::update(Uint32 ticks) { 
  
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

}
