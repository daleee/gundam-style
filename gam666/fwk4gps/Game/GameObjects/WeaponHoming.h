#ifndef _WEAPONHOMING_H_
#define _WEAPONHOMING_H_

class Weapon;
class Player;

class WeaponHoming : public Weapon {
public:
	void fireProjectile();
	WeaponHoming(Player* o, float cdDuration, int mHeat, int hPerShot, float refireTime, iSound* fSound);
	~WeaponHoming();
};

#endif