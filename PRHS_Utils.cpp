/*

Version 2.0.0 of the PRHS Game Library

Last Modified on January 18, 2019

*/

#include "PRHS_Utils.h"

namespace PRHS {

	Input getInputState() {
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0) {
		}
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		Input output;
		output.P1_UP = currentKeyStates[SDL_GetScancodeFromKey(SDLK_UP)];
		output.P1_DOWN = currentKeyStates[SDL_GetScancodeFromKey(SDLK_DOWN)];
		output.P1_LEFT = currentKeyStates[SDL_GetScancodeFromKey(SDLK_LEFT)];
		output.P1_RIGHT = currentKeyStates[SDL_GetScancodeFromKey(SDLK_RIGHT)];
		output.P1_BUTTON_1 = currentKeyStates[SDL_GetScancodeFromKey(SDLK_SPACE)];
		output.P1_BUTTON_2 = currentKeyStates[SDL_GetScancodeFromKey(SDLK_z)];
		output.P1_BUTTON_3 = currentKeyStates[SDL_GetScancodeFromKey(SDLK_x)];

		output.P2_UP = currentKeyStates[SDL_GetScancodeFromKey(SDLK_w)];
		output.P2_DOWN = currentKeyStates[SDL_GetScancodeFromKey(SDLK_s)];
		output.P2_LEFT = currentKeyStates[SDL_GetScancodeFromKey(SDLK_a)];
		output.P2_RIGHT = currentKeyStates[SDL_GetScancodeFromKey(SDLK_d)];
		output.P2_BUTTON_1 = currentKeyStates[SDL_GetScancodeFromKey(SDLK_b)];
		output.P2_BUTTON_2 = currentKeyStates[SDL_GetScancodeFromKey(SDLK_n)];
		output.P2_BUTTON_3 = currentKeyStates[SDL_GetScancodeFromKey(SDLK_m)];

		output.BUTTON_ESC = currentKeyStates[SDL_GetScancodeFromKey(SDLK_ESCAPE)];

		return output;
	}

	void init() {
		static bool wasInit = false; //Static variable to store whether or not the library is already initialized. Maintains state between function calls

		if (!wasInit) { //Check if library already initialized

			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) { //Attempt to initialize SDL
				throw sdl_error(std::string("Error initializing SDL: ") += SDL_GetError()); //Throw exception if SDL cannot initialize
			}

			const int IMG_FLAGS = IMG_INIT_JPG || IMG_INIT_PNG;
			if (IMG_Init(IMG_FLAGS) != IMG_FLAGS) { //Attempt to initialize SDL_image
				throw sdl_error(std::string("Error initializing SDL_image: ") += IMG_GetError()); //Throw exception if SDL_image cannot initialize
			}

			if (TTF_Init() == -1) { //Attempt to initialize SDL_ttf
				throw sdl_error(std::string("Error initializing SDL_ttf: ") += TTF_GetError()); //Throw exception if SDL_ttf cannot initialize
			}

			const int MIXER_FLAGS = MIX_INIT_MP3;
			const int MIXER_SAMPLE_RATE = 44100; //Number of kbps to read from file. Determines sound quality
			const int MIXER_SAMPLE_SIZE = 4096;
			if (Mix_Init(MIXER_FLAGS) != MIXER_FLAGS || Mix_OpenAudio(MIXER_SAMPLE_RATE, MIX_DEFAULT_FORMAT, 1, MIXER_SAMPLE_SIZE) == -1) { //Attempt to initialize SDL_mixer
				throw sdl_error(std::string("Error initializing SDL_mixer: ") += Mix_GetError()); //Throw exception if SDL_mixer does not initialize
			} else {
				Mix_ReserveChannels(4);
			}

			wasInit = true;
		}

	}
}