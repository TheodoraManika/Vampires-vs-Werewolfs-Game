#include "entities.h"

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

Creature::Creature(char symbol): Character(symbol) {}

void Creature::attack(Creature& creature) {

}

void Creature::heal(Creature& creature) {

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

Player::Player(uint x, uint y, bool team) : team(team), Character((team)? 'V': 'W') {

}

void Player::heal(Creature* creature) {

}

uint Player::get_no_potions() const {
	return no_potions;
}