#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "map.h"
#include "general.h"

using namespace std;

extern bool state;	// true -> playing, false -> paused
extern uint result;	// 0 -> exited, 1 -> won, 2 -> lost, 3 -> tied?

Map::Map(uint width, uint height, bool player_team): width(width), height(height), day_night(true), player_team(player_team), low_limit_x(0), low_limit_y(0), up_limit_x(height - 1), up_limit_y(width - 1) {
// limits to be used for the srinking of the borders

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
		/* 
			if the randomly generated position is occupied,
			pick the next one that is empty
		*/
		find_empty_pos(pos_x, pos_y);

		// generating stats
		uint power = rand() % 3 + 1;
		uint defence = rand() % 2 + 1;
		uint med = rand() % 3;
		// saving the vampire
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

		// saving the werewolf
		werewolves.push_back(new Werewolf(pos_x, pos_y, power, defence, med));

		map[pos_x][pos_y] = werewolves.back()->get_symbol();
		creature_map[pos_x][pos_y] = werewolves.back();
	}

	// spawning trees and lakes (none at the edge of the map)
	for (uint i = 1; i < height - 1; i++) {
		for (uint j = 1; j < width - 1; j++) {
			if (map[i][j] == ' ') {
				bool place_tree_lake = ((rand() % 50) == 0);	// 2% spawn rate
				if (place_tree_lake) {
					map[i][j] = (rand() % 10 <= 5) ? '*' : 'o';	
					// tree -> 60% chance, lake -> 40% chance
				}
			}
		}
	}
}

void Map::srink() {
	// srinking will theoretically stop
	if (up_limit_y - low_limit_y <= 3 || up_limit_x - low_limit_x <= 3) return;
	bool move_player = false;	// true if the player should be reallocated due to the srinking borders
	vector<Creature*> to_remove;	// vector of creatures killed by the srinking borders

	// srinking along the longer axis
	if (up_limit_y - low_limit_y >= up_limit_x - low_limit_x) {
		for (int i = low_limit_x; i <= up_limit_x; i++) {
			char ch = player->get_symbol();
			if (map[i][low_limit_y] == ch || map[i][up_limit_y] == ch) move_player = true;
			// X for the map borders
			map[i][low_limit_y] = 'X';
			map[i][up_limit_y] = 'X';
			if (creature_map[i][low_limit_y] != NULL) {	// creature was killed
				to_remove.push_back(creature_map[i][low_limit_y]);
			}
			if (creature_map[i][up_limit_y] != NULL) {
				to_remove.push_back(creature_map[i][up_limit_y]);
			}
		}
		// map srinked
		low_limit_y++;
		up_limit_y--;
	}
	// like above
	else if (up_limit_y - low_limit_y < up_limit_x - low_limit_x) {
		for (int i = low_limit_y; i <= up_limit_y; i++) {
			char ch = player->get_symbol();
			if (map[low_limit_x][i] == ch || map[up_limit_x][i] == ch) move_player = true;
			map[low_limit_x][i] = 'X';
			map[up_limit_x][i] = 'X';
			if (creature_map[low_limit_x][i] != NULL) {
				to_remove.push_back(creature_map[low_limit_x][i]);
				creature_map[low_limit_x][i] = NULL;
			}
			if (creature_map[up_limit_x][i] != NULL) {
				to_remove.push_back(creature_map[up_limit_x][i]);
				creature_map[up_limit_x][i] = NULL;
			}
		}
		low_limit_x++;
		up_limit_x--;
	}
	if (move_player) {
		// find a new position for the player
		uint x = player->get_x();
		uint y = player->get_y();

		find_empty_pos(x, y);
		player->set_x(x);
		player->set_y(y);

		map[x][y] = player->get_symbol();
	}
	// erase the killed werewolves and vampires
	for (auto itr1 = to_remove.begin(); itr1 != to_remove.end(); itr1++) {
		if ((*itr1)->get_symbol() == 'v') {
			for (auto itr2 = vampires.begin(); itr2 != vampires.end(); itr2++) {
				if (*itr1 == *itr2) {
					vampires.erase(itr2);
					break;
				}
			}
		}
		else {
			for (auto itr2 = werewolves.begin(); itr2 != werewolves.end(); itr2++) {
				if (*itr1 == *itr2) {
					werewolves.erase(itr2);
					break;
				}
			}
		}
	}
	// clear the temporary vector
	to_remove.clear();
}

uint& Map::update() {
	static uint calls = 0;	// for the pace of srinking and switching between day and night

	if (!state) return calls;	// paused, don't update

	// within the map borders
	for (int i = low_limit_x; i <= up_limit_x; i++) {
		for (int j = low_limit_y; j <= up_limit_y; j++) {
			if (creature_map[i][j] != NULL) {
				Creature* neighboring_creature = NULL;
				// check if another creature exists around the current creature
				if (i - 1 >= 0 && creature_map[i - 1][j] != NULL) {
					neighboring_creature = creature_map[i - 1][j];
				}
				else if (j - 1 >= 0 && creature_map[i][j - 1] != NULL) {
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

				// if it exists and it is of the same team, choose wether to heal it or not
				if (neighboring_creature->get_symbol() == creature_map[i][j]->get_symbol()) {
					creature_map[i][j]->heal(*neighboring_creature);
				}
				// else if it is of the ooponent team, choose wether to attack it or not
				else {
					creature_map[i][j]->attack(*neighboring_creature);
					// if the neighbouring creature was killed by the attack
					if (neighboring_creature->get_health() <= 0) {
						uint pos_x = neighboring_creature->get_x();
						uint pos_y = neighboring_creature->get_y();

						// clear its former position
						creature_map[pos_x][pos_y] = NULL;
						map[pos_x][pos_y] = ' ';

						// and erase it from the corresponding vector
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
		// empty the vampire's former position on the map
		creature_map[(*i)->get_x()][(*i)->get_y()] = NULL;
		map[(*i)->get_x()][(*i)->get_y()] = ' ';

		(*i)->move((const char**)map, width, height);

		// and update it
		creature_map[(*i)->get_x()][(*i)->get_y()] = *i;
		map[(*i)->get_x()][(*i)->get_y()] = (*i)->get_symbol();
	}

	// as above
	for (auto i = werewolves.begin(); i != werewolves.end(); i++) {
		creature_map[(*i)->get_x()][(*i)->get_y()] = NULL;
		map[(*i)->get_x()][(*i)->get_y()] = ' ';

		(*i)->move((const char**)map, width, height);

		creature_map[(*i)->get_x()][(*i)->get_y()] = *i;
		map[(*i)->get_x()][(*i)->get_y()] = (*i)->get_symbol();
	}

	// player movement
	
	// empty the player's former position on the map
	map[player->get_x()][player->get_y()] = ' ';
	player->move((const char**)map, width, height);
	uint player_x = player->get_x();
	uint player_y = player->get_y();
	// check if the player picked up the extra potion
	if (map[player_x][player_y] == '!') player->pick_up_potion();

	// update the player's position on the map
	map[player_x][player_y] = player->get_symbol();

	// check if the player can heal their team
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

	// day/night change and srinking
	calls++;
	if (calls == 30 || calls == 60) {
		day_night = !day_night;
		if (calls == 60) {
			srink();
			calls = 0;
		}
	}

	// check if game has ended
	if (vampires.size() == 0 || werewolves.size() == 0) {
		this_thread::sleep_for(1s);

		// vampires lost
		if (vampires.size() == 0) {
			if (player_team) result = 2;
			else result = 1;
		}
		// werewolves lost
		else if (werewolves.size() == 0) {
			if (!player_team) result = 2;
			else result = 1;
		}
		// tie (all creatures died)
		else if (vampires.size() == 0 && werewolves.size() == 0) result = 3;
	}

	return calls;	// so the counter can be resetted on replay
}

void Map::find_empty_pos(uint& pos_x, uint& pos_y) {
	if (map[pos_x][pos_y] == ' ') return;
	while (true) {
		if (pos_y < width - 1) pos_y++;
		else if(pos_x < height - 1) {
			// wrap around the map horizontally
			pos_x++;
			pos_y = 0;
		}
		else {
			// wrap around the map diagonally (go back to the first position)
			pos_x = 0;
			pos_y = 0;
		}

		if (map[pos_x][pos_y] == ' ') return;	// empty position found
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
	// clear vampire and werewolf vectors and delete the arrays representing the map
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