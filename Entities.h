#pragma once
#include <vector>
#include "typedefs.h"

using namespace std;

class Character {
protected:
	uint x, y;
	const char symbol;	// for the map representation
public:
	Character(char symbol);
	virtual void move(const char** map, uint width, uint height) = 0;
	char get_symbol() const;
	uint get_x() const;
	uint get_y() const;
};

class Creature;

class Player : public Character {
private:
	uint no_potions;
	bool team;		// true -> vampires, false -> werewolves
public:
	Player(uint x, uint y, bool team);
	void heal(vector<Creature*> creatures);
	void pick_up_potion();
	uint get_no_potions() const;	// return number of potions of the player
	void move(const char** map, uint width, uint height);
};

/* 
Vampire and Werewolf
inherit from Creature
*/

class Creature : public Character {
protected:
	uint power, defence, health, med;	// med -> number of potions of the creature
public:
	Creature(char symbol);
	void attack(Creature& creature);	// argument creature is of opponent team
	void heal(Creature& creature);		// argument creature is of the same team
	uint get_health() const;
	friend void Player::heal(vector<Creature*> creatures);
};

class Vampire : public Creature {
public:
	Vampire(uint x, uint y, uint power, uint defence, uint med);
	void move(const char** map, uint width, uint height);
};

class Werewolf : public Creature {
public:
	Werewolf(uint x, uint y, uint power, uint defence, uint med);
	void move(const char** map, uint width, uint height);
};
