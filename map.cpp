#include <iostream>
#include <cstdlib>
#include "map.h"
#include "typedefs.h"

using namespace std;

extern bool state;	// true -> playing, false -> paused

Map::Map(uint width, uint height, bool player_team): width(width), height(height), day_night(true), player_team(player_team) {

	player = new Player(height - 1, width / 2, player_team);

	// allocating memory for map
	map = new char* [height];
	for (uint i = 0; i < height; i++) {
		map[i] = new char[width];
	}

	srand((uint)time(NULL));	// for the map generation

	/*
	generating the map:
		space -> ground
		* -> tree
		o -> lake
		v -> vampire
		w -> werewolf
		W | V -> player
		! -> potion
	*/

	// placing the player
	map[height - 1][width / 2] = player->get_symbol();
}

void Map::update() {

}

void Map::print() const {
	system("cls");
	system((day_night) ? "Color 70" : "Color 07");

	if (state) {
		// display day_night value
		for (int i = 0; i < ((width + 2) / 2) - 2; i++) cout << ' ';
		cout << (day_night ? "Day" : "Night") << '\n';

		// display map
		for (int i = 0; i < width + 2; i++) cout << '-';
		for (int i = 0; i < height; i++) {
			cout << '|';
			for (int j = 0; j < width; j++) cout << map[i][j];
			cout << '|';
		}
		for (int i = 0; i < width + 2; i++) cout << '-';

		cout << "Press space to pause\t\tPress 0 to exit";
	}
	else {
		// display stats & pause menu
		for (int i = 0; i < ((width + 2) / 2) - 10; i++) cout << ' ';
		cout << "Active Vampires: " << vampires.size() << '\n';
		for (int i = 0; i < ((width + 2) / 2) - 10; i++) cout << ' ';
		cout << "Active Werewolves: " << werewolves.size() << '\n';
		for (int i = 0; i < ((width + 2) / 2) - 10; i++) cout << ' ';
		cout << "Remaining potions: " << player->get_no_potions() << '\n';

		cout << "\n\nPress space to continue\t\tPress 0 to exit\t\tPress R to restart";
	}
}

Map::~Map() {

}