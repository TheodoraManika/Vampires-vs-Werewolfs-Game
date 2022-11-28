#pragma once
#include "typedefs.h"

class Character {
protected:
	uint x, y;
	char symbol;
public:
	virtual void move() = 0;
	char get_symbol() const;
	uint get_x() const;
	uint get_y() const;
};

class Player;

class Creature : public Character {
protected:
	uint attack, defence, health, med;
	bool is_next_to(Creature& creature) const;
	bool can_attack(Creature& creature) const;
public:
	void attack(Creature& creature);
	void heal(Creature& creature);
	friend void Player::heal(Creature* creature);
};

class Werewolf;

class Vampire : public Creature {
	friend class Werewolf;
};

class Werewolf : public Creature {
	friend class Vampire;
};

class Player: public Character {
private:
	uint no_potions;
	bool team;
public:
	void heal(Creature* creature);
};

class Map {
private:
	uint width, height;
	bool day_night;
	char** map;
	Vampire* vampires;
	Werewolf* werewolves;
	Player* player;
public:
	Map(uint width, uint height);
	uint get_width() const;
	uint get_height() const;
	bool get_day_night() const;
};