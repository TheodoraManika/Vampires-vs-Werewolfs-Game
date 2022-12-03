#include "entities.h"
#define CREATURE_HEALTH 3

Character::Character(char symbol): symbol(symbol) {}

char Character::get_symbol() const {
	return symbol;
}

uint Character::get_x() const {
	return x;
}

uint Character::get_y() const {
	return y;
}

Creature::Creature(char symbol): Character(symbol), health(CREATURE_HEALTH) {}

void Creature::attack(Creature& creature) {
	if (creature.power <= power) {
		creature.health -= (power - creature.defence);
	}
}

void Creature::heal(Creature& creature, uint random) {
	if (creature.health < CREATURE_HEALTH && med > 0) {
		if (random % 2) {
			creature.health++;
			med--;
		}
	}
}

uint Creature::get_health() const {
	return health;
}

Vampire::Vampire(uint x, uint y, uint power, uint defence, uint med): Creature('v') {
	this->x = x;
	this->y = y;
	this->power = power;
	this->defence = defence;
	this->med = med;
}

void Vampire::move() {

}

Werewolf::Werewolf(uint x, uint y, uint power, uint defence, uint med): Creature('w') {
	this->x = x;
	this->y = y;
	this->power = power;
	this->defence = defence;
	this->med = med;
}

void Werewolf::move() {

}

Player::Player(uint x, uint y, bool team) : team(team), Character((team)? 'V': 'W'), no_potions(0) {

}

void Player::heal(Creature* creature) {

}

void Player::pick_up_potion() {
	no_potions++;
}

uint Player::get_no_potions() const {
	return no_potions;
}

void Player::move() {

}