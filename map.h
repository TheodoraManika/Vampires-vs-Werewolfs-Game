#pragma once
#include <vector>
#include "typedefs.h"
#include "entities.h"

using namespace std;

class Map {
private:
	const uint width, height;
	bool day_night;		// true -> day, false -> night
	vector<Vampire*> vampires;
	vector<Werewolf*> werewolves;
	Player* player;
	char** map;		// representation of the map
public:
	Map(uint width, uint height);
	void update();		// updates the state of the game
	void print() const;		// provides graphical interface
	~Map();
};