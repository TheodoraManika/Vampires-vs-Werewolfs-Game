#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "map.h"
#include "general.h"

using namespace std;

extern bool state;	// true -> playing, false -> paused
extern uint result;

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

	creature_map = new Creature** [height];
	for (uint i = 0; i < height; i++) {
		creature_map[i] = new Creature* [width];
		for (uint j = 0; j < width; j++) {
			creature_map[i][j] = NULL;
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
	map[player->get_x()][player->get_y()] = player->get_symbol();
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
		creature_map[pos_x][pos_y] = vampires.back();
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
		creature_map[pos_x][pos_y] = werewolves.back();
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

uint& Map::update() {
	static uint calls = 0;

	if (!state) return calls;

	for (uint i = 0; i < height; i++) {
		for (uint j = 0; j < width; j++) {
			if (creature_map[i][j] != NULL) {
				Creature* neighboring_creature = NULL;
				if ((signed)i - 1 >= 0 && creature_map[i - 1][j] != NULL) {
					neighboring_creature = creature_map[i - 1][j];
				}
				else if ((signed)j - 1 >= 0 && creature_map[i][j - 1] != NULL) {
					neighboring_creature = creature_map[i][j - 1];
				}
				else if (i + 1 < height && creature_map[i + 1][j] != NULL) {
					neighboring_creature = creature_map[i + 1][j];
				}
				else if (j + 1 < width && creature_map[i][j + 1] != NULL) {
					neighboring_creature = creature_map[i][j + 1];
				}

				if (neighboring_creature == NULL)
					continue;

				if (neighboring_creature->get_symbol() == creature_map[i][j]->get_symbol()) {
					creature_map[i][j]->heal(*neighboring_creature);
				}
				else {
					creature_map[i][j]->attack(*neighboring_creature);
					if (neighboring_creature->get_health() <= 0) {
						uint pos_x = neighboring_creature->get_x();
						uint pos_y = neighboring_creature->get_y();

						creature_map[pos_x][pos_y] = NULL;
						map[pos_x][pos_y] = ' ';

						if (neighboring_creature->get_symbol() == 'v') {
							for (auto itr = vampires.begin(); itr != vampires.end(); itr++) {
								if (neighboring_creature == *itr) {
									vampires.erase(itr);
									break;
								}
							}
						}
						else {
							for (auto itr = werewolves.begin(); itr != werewolves.end(); itr++) {
								if (neighboring_creature == *itr) {
									werewolves.erase(itr);
									break;
								}
							}
						}

						delete neighboring_creature;
					}
				}
			}
		}
	}

	// creature movement
	for (auto i = vampires.begin(); i != vampires.end(); i++) {
		creature_map[(*i)->get_x()][(*i)->get_y()] = NULL;
		map[(*i)->get_x()][(*i)->get_y()] = ' ';

		(*i)->move((const char**)map, width, height);

		creature_map[(*i)->get_x()][(*i)->get_y()] = *i;
		map[(*i)->get_x()][(*i)->get_y()] = (*i)->get_symbol();
	}

	for (auto i = werewolves.begin(); i != werewolves.end(); i++) {
		creature_map[(*i)->get_x()][(*i)->get_y()] = NULL;
		map[(*i)->get_x()][(*i)->get_y()] = ' ';

		(*i)->move((const char**)map, width, height);

		creature_map[(*i)->get_x()][(*i)->get_y()] = *i;
		map[(*i)->get_x()][(*i)->get_y()] = (*i)->get_symbol();
	}

	// player movement
	map[player->get_x()][player->get_y()] = ' ';
	player->move((const char**)map, width, height);
	uint player_x = player->get_x();
	uint player_y = player->get_y();
	if (map[player_x][player_y] == '!') player->pick_up_potion();
	map[player_x][player_y] = player->get_symbol();

	if (day_night && player_team || !day_night && !player_team) {
		if (player_team) {
			player->heal(vampires);
		}
		else
		{
			player->heal(werewolves);
		}
	}

	using namespace chrono_literals;

	if (vampires.size() == 0 || werewolves.size() == 0) {
		this_thread::sleep_for(1s);

		if (vampires.size() == 0) {
			if (player_team) result = 2;
			else result = 1;
		}
		else if (werewolves.size() == 0) {
			if (!player_team) result = 2;
			else result = 1;
		}
		else if (vampires.size() == 0 && werewolves.size() == 0) result = 3;

		return calls;
	}

	calls++;
	if (calls == 30) {
		day_night = !day_night;
		calls = 0;
	}

	return calls;
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
		cout << "Active Vampires: " << vampires.size() << '\n';
		cout << "Active Werewolves: " << werewolves.size() << '\n';
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

	for (uint i = 0; i < height; i++) {
		delete[] creature_map[i];
	}
	delete[] creature_map;
}