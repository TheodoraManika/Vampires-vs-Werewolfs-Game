#pragma once
#include <vector>
#include "typedefs.h"
#include "entities.h"

using namespace std;

class Map {
private:
	const uint width, height;
	bool day_night;		// true -> day, false -> night
	bool player_team;	// true -> vampires, false -> werewolves
	vector<Vampire*> vampires;
	vector<Werewolf*> werewolves;
	Player* player;
	char** map;		// representation of the map
public:
	Map(uint width, uint height, bool player_team);
	void update();		// updates the state of the game
	void find_empty_pos(uint& pos_x, uint& pos_y); /* finds an empty
	position on the map in case of collisions during object generation
	*/
	void print() const;		// provides graphical interface
	~Map();
};