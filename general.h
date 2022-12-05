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
	bool H;
	bool SPACE;
	bool ZERO;
	bool R;

	void operator=(bool value) {
		W = value;
		A = value;
		S = value;
		D = value;
		UP_ARROW = value;
		LEFT_ARROW = value;
		DOWN_ARROW = value;
		RIGHT_ARROW = value;
		H = value;
		SPACE = value;
		ZERO = value;
		R = value;
	}
};