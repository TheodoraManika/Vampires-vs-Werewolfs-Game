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

bool state = true;

int main() {
	srand((uint)time(NULL));
	using namespace chrono_literals;

	uint width, height;
	bool team;

	system("color 04");
	cout << "Werewolves Vs Vampires\a";
	this_thread::sleep_for(3s);
	system("cls");
	system("color 07");
	cout << "Display instructions (Y/n)? ";
	string answer;
	cin >> answer;
	if (answer[0] == 'y' || answer[0] == 'Y')
		instructions();
	play_intro(width, height, team);

	Map* map = new Map(width, height, team);
	while (true) {
		key_state = false;
		for (int i = 0; i < 10; i++) {
			this_thread::sleep_for(50ms);

			if (GetKeyState(VK_SPACE) & 0x8000) {
				key_state.SPACE = true;
				state = !state;
			}
			if ((GetKeyState(0x30) & 0x8000) || (GetKeyState(VK_NUMPAD0) & 0x8000)) {
				key_state.ZERO = true;
				ending();
				goto exit;
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
		map->print();
	}

	exit:
	return 0;
}