#pragma once
#include <vector>
#include "general.h"
#include "entities.h"

using namespace std;

class Map {
private:
	const uint width, height;
	uint low_limit_x, low_limit_y, up_limit_x, up_limit_y;	// limits of the srinking map
	bool day_night;		// true -> day, false -> night
	bool player_team;	// true -> vampires, false -> werewolves
	vector<Creature*> vampires;
	vector<Creature*> werewolves;
	Player* player;
	char** map;		// representation of the map
	Creature*** creature_map;	// for faster update
	void find_empty_pos(uint& pos_x, uint& pos_y); /* finds an empty
	position on the map in case of collisions during object generation
	*/
	void srink();	// srink the map
public:
	Map(uint width, uint height, bool player_team);
	uint& update();		// updates the state of the game
	void print() const;		// provides graphical interface
	~Map();
};