#include <iostream>
#include <cstdlib>
#include "map.h"
#include "general.h"

using namespace std;

extern bool state;	// true -> playing, false -> paused

Map::Map(uint width, uint height, bool player_team): width(width), height(height), day_night(true), player_team(player_team) {

	player = new Player(height - 1, width / 2, player_team);
	player->pick_up_potion();

	// allocating memory for map
	map = new char* [height];
	for (uint i = 0; i < height; i++) {
		map[i] = new char[width];
		for (uint j = 0; j < width; j++) {
			map[i][j] = ' ';
		}
	}

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
	// placing the potion
	uint pos_x = rand() % height;
	uint pos_y = rand() % width;
	find_empty_pos(pos_x, pos_y);
	map[pos_x][pos_y] = '!';

	uint team_size = (width + height) / 5;
	// spawning vampires
	for (uint i = 0; i < team_size; i++) {
		pos_x = rand() % height;
		pos_y = rand() % width;
		find_empty_pos(pos_x, pos_y);

		uint power = rand() % 3 + 1;
		uint defence = rand() % 2 + 1;
		uint med = rand() % 3;
		vampires.push_back(new Vampire(pos_x, pos_y, power, defence, med));

		map[pos_x][pos_y] = vampires.back()->get_symbol();
	}

	// spawning werewolves
	for (uint i = 0; i < team_size; i++) {
		pos_x = rand() % height;
		pos_y = rand() % width;
		find_empty_pos(pos_x, pos_y);

		uint power = rand() % 3 + 1;
		uint defence = rand() % 2 + 1;
		uint med = rand() % 3;
		werewolves.push_back(new Werewolf(pos_x, pos_y, power, defence, med));

		map[pos_x][pos_y] = werewolves.back()->get_symbol();
	}

	// spawning trees and lakes (none at the edge of the map)
	for (uint i = 1; i < height - 1; i++) {
		for (uint j = 1; j < width - 1; j++) {
			if (map[i][j] == ' ') {
				bool place_tree_lake = ((rand() % 50) == 0);
				if (place_tree_lake) {
					map[i][j] = (rand() % 10 <= 5) ? '*' : 'o';
				}
			}
		}
	}
}

void Map::update() {

}

void Map::find_empty_pos(uint& pos_x, uint& pos_y) {
	if (map[pos_x][pos_y] == ' ') return;
	while (true) {
		if (pos_y < width - 1) pos_y++;
		else if(pos_x < height - 1) {
			pos_x++;
			pos_y = 0;
		}
		else {
			pos_x = 0;
			pos_y = 0;
		}

		if (map[pos_x][pos_y] == ' ') return;
	}
}

void Map::print() const {
	system("cls");
	system((day_night) ? "Color 70" : "Color 07");

	if (state) {
		// display day_night value
		for (uint i = 0; i < ((width + 2) / 2) - 2; i++) cout << ' ';
		cout << (day_night ? "Day" : "Night") << '\n';

		// display map
		for (uint i = 0; i < width + 2; i++) cout << '-';
		cout << '\n';
		for (uint i = 0; i < height; i++) {
			cout << '|';
			for (uint j = 0; j < width; j++) cout << map[i][j];
			cout << '|';
			cout << '\n';
		}
		for (uint i = 0; i < width + 2; i++) cout << '-';

		cout << "\n\nPress space to pause\t\tPress 0 to exit";
	}
	else {
		// display stats & pause menu
		for (uint i = 0; i < ((width + 2) / 2) - 10; i++) cout << ' ';
		cout << "Active Vampires: " << vampires.size() << '\n';
		for (uint i = 0; i < ((width + 2) / 2) - 10; i++) cout << ' ';
		cout << "Active Werewolves: " << werewolves.size() << '\n';
		for (uint i = 0; i < ((width + 2) / 2) - 10; i++) cout << ' ';
		cout << "Remaining potions: " << player->get_no_potions() << '\n';

		cout << "\n\nPress space to continue\t\tPress 0 to exit\t\tPress R to restart";
	}
}

Map::~Map() {
	for(auto itr = vampires.begin(); itr != vampires.end(); itr++) {
		delete* itr;
	}
	vampires.clear();
	for (auto itr = werewolves.begin(); itr != werewolves.end(); itr++) {
		delete* itr;
	}
	werewolves.clear();
	delete player;
	for (uint i = 0; i < height; i++) {
		delete[] map[i];
	}
	delete[] map;
}