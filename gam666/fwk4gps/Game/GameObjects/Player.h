#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "..\GameObject.h"
#include "..\PhysicsObject.h"
#include "InputComponent.h"
#include "Weapon.h"
#include "Projectile.h"
#include "GameTimer.h"

class World;
class iCamera;
class iGraphic;
class iSound;
class Projectile;
class Weapon;

class Player : public GameObject {
  InputComponent input;
  iCamera* camera;
  iSound* jumpSound;
  iSound* deathSound;
  iSound* beenHitSound;
  Vector cameraDistance;

  void initSounds();
public:
  int thrusterCooldown;
  int thruster;
  int id;
  //
  int health;
  int startingHealth;
  int kills;
  std::vector<float> deaths;
  bool isAlive;
  Weapon* weaponSet[3];
  GameTimer lifeTimer;
  GameTimer respawnTimer;

  Player(World* world, int id, iGraphic* graphic = nullptr);
  ~Player();
  void update();
  void useThruster(int amount);
  void recoverThrusters();
  int hitBoundary() { return stayInBounds(); };
  void createCamera();
  virtual void onCollision(Projectile* projectile);
  void setWeapons();
  Vector findSpawnPoint();
  void die();
  void respawn();
  
  iCamera* getCamera() { return camera; };
};

#endif