#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <thread>
#include <Windows.h>
#include "general.h"
#include "interface.h"
#include "map.h"

using namespace std;

keys key_state;

bool state;		// true -> playing, false -> paused
uint result;	// 0 -> exited, 1 -> won, 2 -> lost, 3 -> tied?

int main() {
	srand((uint)time(NULL));
	using namespace chrono_literals;

	uint width, height;
	bool team;

	system("color 04");
	cout << "Werewolves Vs Vampires\a";	// game title
	this_thread::sleep_for(3s);
	system("cls");
	system("color 07");
	// ask wether to display instructions
	cout << "Display instructions (Y/n)? ";
	string answer;
	cin >> answer;
	if (answer[0] == 'y' || answer[0] == 'Y')
		instructions();

replay:	// label for easier control
	result = 0;
	state = true;
	play_intro(width, height, team);

	Map* map = new Map(width, height, team);
	while (true) {
		key_state = false;
		for (int i = 0; i < 10; i++) {
			this_thread::sleep_for(50ms);
			// 500 ms window for user input

			if (GetKeyState(VK_SPACE) & 0x8000) {
				state = !state;
			}
			if ((GetKeyState(0x30) & 0x8000) || (GetKeyState(VK_NUMPAD0) & 0x8000)) {
				ending(result);
				delete map;
				goto exit;	// break from nested loop
			}
			if ((GetKeyState('R') & 0x8000) && state == false) {
				map->update() = 0;	// reset day/night timer
				delete map;
				// flush leftover characters from player input
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				goto replay;
			}
			if (GetKeyState(VK_UP) & 0x8000) {
				key_state.UP_ARROW = true;
			}
			if (GetKeyState(VK_LEFT) & 0x8000) {
				key_state.LEFT_ARROW = true;
			}
			if (GetKeyState(VK_DOWN) & 0x8000) {
				key_state.DOWN_ARROW = true;
			}
			if (GetKeyState(VK_RIGHT) & 0x8000) {
				key_state.RIGHT_ARROW = true;
			}
			if (GetKeyState('W') & 0x8000) {
				key_state.W = true;
			}
			if (GetKeyState('A') & 0x8000) {
				key_state.A = true;
			}
			if (GetKeyState('S') & 0x8000) {
				key_state.S = true;
			}
			if (GetKeyState('D') & 0x8000) {
				key_state.D = true;
			}
			if (GetKeyState('H') & 0x8000) {
				key_state.H = true;
			}
		}
		map->update();
		if (result != 0) {
			ending(result);
			break;
		}
		map->print();
	}

exit:
	return 0;
}