#pragma once

/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include <string>

//SDL Includes
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL2_rotozoom.h"

#include "PRHS_Exception.h"

namespace PRHS {

	const int VOID = -201769420;

	struct Rect {
		int x;
		int y;
		int w;
		int h;
		int r;
		explicit operator SDL_Rect() const;
	};

	struct FloatRect {
		double x;
		double y;
		double w;
		double h;
		double r;
		explicit operator Rect() const;
	};

	struct Vector2 {
		double x;
		double y;
	};

	struct Input {
		bool P1_BUTTON_1;
		bool P1_BUTTON_2;
		bool P1_BUTTON_3;
		bool P1_UP;
		bool P1_DOWN;
		bool P1_LEFT;
		bool P1_RIGHT;

		bool P2_BUTTON_1;
		bool P2_BUTTON_2;
		bool P2_BUTTON_3;
		bool P2_UP;
		bool P2_DOWN;
		bool P2_LEFT;
		bool P2_RIGHT;

		bool BUTTON_ESC;
	};


	//Miscelleneous Functions
	Input getInputState();
	void init();
};
