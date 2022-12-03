#include "entities.h"
#define CREATURE_HEALTH 3

extern keys key_state;

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

void Creature::heal(Creature& creature) {
	if (creature.health < CREATURE_HEALTH && med > 0) {
		if (rand() % 2) {
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

void Vampire::move(const char** map, uint width, uint height) {
	int relative_x = rand() % 3 - 1;
	int relative_y = rand() % 3 - 1;
	if (relative_x == 0 && relative_y == 0) return; // don't move
	int new_x = relative_x + x;
	int new_y = relative_y + y;
	if (new_x < height && new_x >= 0 && new_y < width && new_y >= 0) {
		if (map[new_x][new_y] == ' ') {
			x = new_x;
			y = new_y;
			return;
		}
	}
	for (uint i = x - 1; i <= x + 1; i++) {
		for (uint j = y - 1; j <= y + 1; j++) {
			if (i < height && i >= 0 && j < width && j >= 0) {
				if (map[i][j] == ' ') {
					x = i;
					y = j;
					return;
				}
			}
		}
	}
}

Werewolf::Werewolf(uint x, uint y, uint power, uint defence, uint med): Creature('w') {
	this->x = x;
	this->y = y;
	this->power = power;
	this->defence = defence;
	this->med = med;
}

void Werewolf::move(const char** map, uint width, uint height) {
	if (rand() % 9 == 0) return;	// don't move

	bool axis = (rand() % 2) ? true : false;	// true -> horizontal move, false -> vertical move
	bool direction = (rand() % 2) ? true : false;	// true -> forwards, false -> backwards

	uint new_x = x + ((!axis) ? (direction ? 1 : -1) : 0);
	uint new_y = y + ((axis) ? (direction ? 1 : -1) : 0);

	if (new_x < height && new_x >= 0 && new_y < width && new_y >= 0) {
		if (map[new_x][new_y] == ' ') {
			x = new_x;
			y = new_y;
			return;
		}
	}
	new_x = x - 1;
	new_y = y;
	for (int i = 0; i < 4; i++) {
		if (new_x < height && new_x >= 0 && new_y < width && new_y >= 0) {
			if (map[new_x][new_y] == ' ') {
				x = new_x;
				y = new_y;
				return;
			}
		}
		new_x = new_x + ((i % 2 == 0) ? 1 : 0);
		new_y = new_y + ((i % 2 == 0) ? -1 : 2);
	}
}

Player::Player(uint x, uint y, bool team) : team(team), Character((team)? 'V': 'W'), no_potions(0) {

}

void Player::heal(vector<Creature*> creatures) {
	for (auto i = creatures.begin(); i != creatures.end(); i++) {
		(*i)->health = CREATURE_HEALTH;
	}
}

void Player::pick_up_potion() {
	no_potions++;
}

uint Player::get_no_potions() const {
	return no_potions;
}

void Player::move(const char** map, uint width, uint height) {
	if (key_state.W || key_state.UP_ARROW) {
		if (x - 1 >= 0) {
			if (map[x - 1][y] == ' ') {
				x--;
				return;
			}
		}
	}
	if (key_state.A || key_state.LEFT_ARROW) {
		if (y - 1 >= 0) {
			if (map[x][y - 1] == ' ') {
				y--;
				return;
			}
		}
	}
	if (key_state.S || key_state.DOWN_ARROW) {
		if (x + 1 < height) {
			if (map[x + 1][y] == ' ') {
				x++;
				return;
			}
		}
	}
	if (key_state.D || key_state.RIGHT_ARROW) {
		if (y + 1 < width) {
			if (map[x][y + 1] == ' ') {
				y++;
				return;
			}
		}
	}
}