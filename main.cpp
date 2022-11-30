#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>
#include "typedefs.h"
#include "interface.h"
#include "map.h"

using namespace std;

bool state;

int main() {
	using namespace chrono_literals;

	uint width, height;
	bool team;
	system("color 04");
	cout << "Werewolves Vs Vampires\a";
	this_thread::sleep_for(3s);

	system("cls");
	system("color 07");
	cout << "Display instructions (Y/n)? ";
	char answer;
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
		instructions();
	play_intro(width, height, team);

	return 0;
}