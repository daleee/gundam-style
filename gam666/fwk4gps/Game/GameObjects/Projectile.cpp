#include "Projectile.h"
#include "..\World.h"
#include "../../MathDef.h"
#include "../../Camera.h"
#include <cmath>

#define AIMASSISTDISTANCE 75
#define PI 3.14159265f

Projectile::Projectile(World* world, Player* owner, iGraphic* model, float pSpeed, float lifespan, int damagePerProjectile, bool iHoming, 
  float mHomeAngle) : 
    GameObject(world, model), owner(owner), pSpeed(pSpeed), life(lifespan), damage(damagePerProjectile),
    time(0), force(1000), target(nullptr), maxHomeAngle(10.0f) {
  isHoming = iHoming;
  maxHomeAngle = mHomeAngle;

	if (owner) initializeFromOwner();
}

void Projectile::shoot()
{
	world->projectiles.push_back(this);
	world->add(this);
}

void Projectile::onCollision(Player* other)
{
  //debug("PLAYER HIT\n");
	//override for special hit effects here
  if((other->health - damage) <= 0 && other->isAlive)
  {
    owner->kills++;
    other->die();
  }
	world->remove(this);
}

void Projectile::onCollision(GameObject* other)
{
	//override for special hit effects here
	world->remove(this);
}

void Projectile::update() {
  ++time;
  if (time >= life) {
    world->remove(this);
  } else {
    if(isHoming && time > 20) {
      homeOnTarget();
    }
    speed = pSpeed * direction;
    GameObject::update();
  }
}

void Projectile::findTarget() {
  Camera* ownerCamera = (Camera*)owner->getCamera();
  float minDistance = 9999999.0f;
  for(unsigned i = 0; i < world->players.size(); i++) {
    Vector playerWorldPosition = world->players[i]->position();
    if(world->players[i] != owner) {
      float dotPlayerOwner = dot(playerWorldPosition - owner->position(), ownerCamera->orientation('z'));
      float normPlayerOwner = (playerWorldPosition - owner->position()).length();
      float dist = sqrt(normPlayerOwner * normPlayerOwner - dotPlayerOwner * dotPlayerOwner);

      if(((dist < AIMASSISTDISTANCE) && (dotPlayerOwner > 0)) || isHoming ) {
        if((dist < minDistance) && world->players[i]->isAlive) {
          minDistance = dist;
          target = world->players[i];
        }
      }
    }
  }
}

void Projectile::homeOnTarget() {
  if(target){
    bool negativeAngle = false;
    Vector previousDirection = direction;
    Vector newDirection = (target->position() - this->position()) / (target->position() - this->position()).length();
  
    float testNegative = dot(previousDirection, newDirection);
    if(testNegative < 0)
    {
      negativeAngle = true;
      newDirection = -1 * newDirection;
      newDirection.y = -1 * newDirection.y;
    }

    Vector projectionNewOnPrevious = (dot(previousDirection, newDirection) / previousDirection.length()) * previousDirection;

    // angle = arccos((previousDirection dot newDirection) / (|previousDirection||newDirection|))
    float angle = acos((dot(projectionNewOnPrevious, newDirection)) / (projectionNewOnPrevious.length() * newDirection.length())) * 180.0f / PI;

    if(angle > maxHomeAngle || negativeAngle) {
      Vector opposite = newDirection - projectionNewOnPrevious;
      Vector oppositeUnitVector = opposite / opposite.length();
      float magnitudeOpposite = opposite.length();
      float newMagnitudeOpposite = projectionNewOnPrevious.length() * tan(maxHomeAngle * PI / 180.0f);
      Vector newOpposite = newMagnitudeOpposite * oppositeUnitVector;

      if(negativeAngle) {
        newOpposite = -1 * newOpposite;
        newOpposite.y = -1 * newOpposite.y;
      }

      Vector newHypoteneuse = newOpposite + projectionNewOnPrevious;
      Vector newUnitVector = newHypoteneuse / newHypoteneuse.length();

      direction = newDirection.length() * newUnitVector;
    } else {
      direction = newDirection;
    }
  }
}

void Projectile::initializeFromOwner() {
  findTarget();
  if(target && !isHoming) {
    direction = (target->position() - owner->position()) / (target->position() - owner->position()).length();
  } else {
	  direction = owner->getCamera()->orientation('z');
  }
	setMatrix(owner->rotation());
  setTranslation(owner->position());
}

//rotates around the yaw axis to change direction of the projectile 
void Projectile::shiftDirection(float rad) {  
  Matrix tmp;
  rad    *= COORD_SYSTEM;
  float c = cosf(rad);
  float s = sinf(rad);	
  Matrix rot(c, 0, s, 0,
    0, 1, 0, 0,
    -s, 0, c, 0,
    0, 0, 0, 1);
  direction *= rot;
}

Projectile::~Projectile() {

}
