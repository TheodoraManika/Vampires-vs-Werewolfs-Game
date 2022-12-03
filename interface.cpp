#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>
#include "interface.h"

using namespace std;

void play_intro(uint& width, uint& height, bool& team) {
	using namespace chrono_literals;

	system("cls");
	system("Color 04");
	cout << "WELCOME" << "\n";
	
	this_thread::sleep_for(2s);
	system("cls");
	
	while (true) {
		try {
			system("Color 0A");
			cout << "Please give map dimensions" << "\n";
			cout << "width: ";
			cin >> width;
			cout << "height: ";
			cin >> height;

			if (width == 0 || height == 0 || width + height < 5) {
				throw(width);
			}

			break;
		}
		catch (...) {
			system("cls");
			system("Color 07");
			if (width == 0) {
				cout << "Width can't be 0";
			}
			else if (height == 0) {
				cout << "Height can't be 0";
			}
			else {
				cout << "Sum of dimensions must be greater than or equal to 5";
			}
			this_thread::sleep_for(3s);
			system("cls");
		}
	}

	this_thread::sleep_for(1s);
	system("cls");

	system("Color 07");
	cout << "width: " << width << " height: " << height << "\n";

	this_thread::sleep_for(2s);
	system("cls");

	system("Color 0A");
	cout << "Time to choose your team" << "\n";

	this_thread::sleep_for(2500ms);
	system("cls");
	system("Color 04");

	cout << "Press V for Vampires or W for Werewolves" << "\n";

	while (true) {
		this_thread::sleep_for(50ms);

		if (GetKeyState('V') & 0x8000) {
			team = true;
			break;
		}
		else if (GetKeyState('W') & 0x8000) {
			team = false;
			break;
		}
	}
	
	system("cls");

	cout << "You chose: " << ((team) ? "Vampires" : "Werewolves");
	this_thread::sleep_for(2s);
	system("cls");

	// countdown
	const char* colors[3] = { "Color 06", "Color 0A", "Color 07" };
	for (int k = 0; k < 3; k++) {
		for (int i = 1; i <= 10; i++) {
			cout << 3 - k;
			for (int j = 0; j < i; j++) {
				cout << ' ';
			}
			cout << '-';
			this_thread::sleep_for(100ms);
			system("cls");
		}
		system(colors[k]);
	}
}

void instructions() {
	using namespace chrono_literals;

	system("cls");
	system("Color 04");

	cout << "INSTRUCTIONS";

	this_thread::sleep_for(2s);
	system("cls");
	system("Color 0A");

	cout << "Keys controlling the state of the game";

	this_thread::sleep_for(2s);
	system("cls");
	system("Color 0F");

	cout << "Space\t:\tpause/continue\n"
		<< "0\t:\texit\n"
		<< "R\t:\trestart (in pause)\n\n";

	system("pause");
	system("cls");
	system("Color 0A");

	cout << "Keys to move avatar";

	this_thread::sleep_for(2s);
	system("cls");
	system("Color 0F");

	cout << "LEFT_ARROW\t:\tmove left\n"
		<< "RIGHT_ARROW\t:\tmove right\n"
		<< "UP_ARROW\t:\tmove forward\n"
		<< "DOWN_ARROW\t:\tmove backwards\n\n";

	system("pause");
	system("cls");
	system("Color 0A");

	cout << "OR";

	this_thread::sleep_for(2s);
	system("cls");
	system("Color 0F");

	cout << "A : move left\n"
		<< "D : move right\n"
		<< "W : move forward\n"
		<< "S : move backwards\n\n";
	system("pause");
}