#include <iostream>
#include <cstdlib>
#include "map.h"
#include "typedefs.h"

using namespace std;

Map::Map(uint width, uint height): width(width), height(height), day_night(true) {
	// allocating memory for map
	map = new char* [height];
	for (uint i = 0; i < height; i++) {
		map[i] = new char[width];
	}


}

void Map::update() {

}

void Map::print() const {

}

Map::~Map() {

}