#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <vector>

#pragma once

typedef unsigned int uint;

struct keys {
	bool W;
	bool A;
	bool S;
	bool D;
	bool UP_ARROW;
	bool LEFT_ARROW;
	bool DOWN_ARROW;
	bool RIGHT_ARROW;
	bool SPACE;
	bool ZERO;
	bool R;
};