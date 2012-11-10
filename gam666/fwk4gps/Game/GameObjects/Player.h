#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "..\GameObject.h"
#include "..\PhysicsObject.h"
#include "InputComponent.h"
#include "Weapon.h"
#include "Projectile.h"

class World;
class iCamera;
class Projectile;
class Weapon;

class Player : public GameObject {
  InputComponent input;
  iCamera* camera;
  Vector cameraDistance;
public:
  int thrusterCooldown;
  int thruster;
  int id;
  //
  int health;
  Weapon* weaponSet[3];

  Player(World* world, int id);
  ~Player();
  void update();
  void useThruster(int amount);
  void recoverThrusters();
  void createCamera();
  //
  void onHit(Projectile* projectile);
  bool isAlive();
  void setWeapons();
  
  iCamera* getCamera() { return camera; };
};

#endif