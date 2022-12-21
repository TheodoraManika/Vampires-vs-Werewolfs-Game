#include "entities.h"
#define CREATURE_HEALTH 3

extern keys key_state;	// for actions of the player

Character::Character(char symbol, uint x, uint y): symbol(symbol), x(x), y(y) {}

char Character::get_symbol() const {
	return symbol;
}

uint Character::get_x() const {
	return x;
}

uint Character::get_y() const {
	return y;
}

Creature::Creature(char symbol, uint x, uint y, uint power, uint defence, uint med): Character(symbol, x, y), power(power), defence(defence), med(med), health(CREATURE_HEALTH) {}

void Creature::attack(Creature& creature) {
	if (creature.power <= power) {
		creature.health -= max((signed)(power - creature.defence), 1);
		// guaranteed damage of 1
	}
}

void Creature::heal(Creature& creature) {
	if (creature.health < CREATURE_HEALTH && med > 0) {
		if (rand() % 2) {
			// 50% chance that a creature decides to heal a neighbouring teammate
			creature.health++;
			med--;
		}
	}
}

int Creature::get_health() const {
	return health;
}

// using Creature's constructor
Vampire::Vampire(uint x, uint y, uint power, uint defence, uint med): Creature('v', x, y, power, defence, med) {}

void Vampire::move(const char** map, uint width, uint height) {
	// choose a new position
	int relative_x = rand() % 3 - 1;
	int relative_y = rand() % 3 - 1;
	if (relative_x == 0 && relative_y == 0) return; // don't move
	int new_x = relative_x + (signed)x;
	int new_y = relative_y + (signed)y;
	/*
		relative positions chart:
		(-1, -1)	(-1, 0)		(-1, 1)
		(0, -1)		(0, 0)		(0, 1)
		(1, -1)		(1, 0)		(1, 1)
	*/
	// check if the new position is within map limits and empty
	if (new_x < height && new_x >= 0 && new_y < width && new_y >= 0) {
		if (map[new_x][new_y] == ' ') {
			x = new_x;
			y = new_y;
			return;
		}
	}
	// if it is not, search for any empty positions around the Vampire
	for (int i = (signed)x - 1; i <= x + 1; i++) {
		for (int j = (signed)y - 1; j <= y + 1; j++) {
			// same conditions as in lines 60 and 61
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

Werewolf::Werewolf(uint x, uint y, uint power, uint defence, uint med): Creature('w', x, y, power, defence, med) {}

void Werewolf::move(const char** map, uint width, uint height) {
	if (rand() % 9 == 0) return;	// don't move (same chance as for the vampires)

	bool axis = (rand() % 2) ? true : false;	// true -> horizontal move (y), false -> vertical move (x)
	bool direction = (rand() % 2) ? true : false;	// true -> forwards (++), false -> backwards (--)

	int new_x = (signed)x + ((!axis) ? (direction ? 1 : -1) : 0);
	int new_y = (signed)y + ((axis) ? (direction ? 1 : -1) : 0);

	// check if the new position is within map limits and empty
	if (new_x < height && new_x >= 0 && new_y < width && new_y >= 0) {
		if (map[new_x][new_y] == ' ') {
			x = new_x;
			y = new_y;
			return;
		}
	}
	// if it is not, check all 4 possible new positions
	new_x = (signed)x - 1;
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
		/*
			Order of search:
			|-----------|
			|   | 1 |   |
			| 2 | x | 3 |
			|   | 4 |   |
			|-----------|
		*/
	}
}

Player::Player(uint x, uint y, bool team) : team(team), Character((team)? 'V': 'W', x, y), no_potions(0) {}

void Player::heal(vector<Creature*> creatures) {
	if (!key_state.H || no_potions == 0) return;	// can't heal team
	for (auto i = creatures.begin(); i != creatures.end(); i++) {	// for every creature in team
		(*i)->health = CREATURE_HEALTH;	// fully healed
	}
	no_potions--;
}

void Player::pick_up_potion() {
	no_potions++;
}

uint Player::get_no_potions() const {
	return no_potions;
}

void Player::set_x(uint x) {
	this->x = x;
}

void Player::set_y(uint y) {
	this->y = y;
}

void Player::move(const char** map, uint width, uint height) {
	// player can only move to empty spaces or the position of the extra potion (!)
	if (key_state.W || key_state.UP_ARROW) {
		if ((signed)x - 1 >= 0) {
			if (map[x - 1][y] == ' ' || map[x - 1][y] == '!') {
				x--;
				return;
			}
		}
	}
	if (key_state.A || key_state.LEFT_ARROW) {
		if ((signed)y - 1 >= 0) {
			if (map[x][y - 1] == ' ' || map[x][y - 1] == '!') {
				y--;
				return;
			}
		}
	}
	if (key_state.S || key_state.DOWN_ARROW) {
		if (x + 1 < height) {
			if (map[x + 1][y] == ' ' || map[x + 1][y] == '!') {
				x++;
				return;
			}
		}
	}
	if (key_state.D || key_state.RIGHT_ARROW) {
		if (y + 1 < width) {
			if (map[x][y + 1] == ' ' || map[x][y + 1] == '!') {
				y++;
				return;
			}
		}
	}
}