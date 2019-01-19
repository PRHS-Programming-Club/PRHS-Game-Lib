#pragma once

/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include <vector>
#include <string>
#include <memory>

#include "PRHS_Utils.h"
#include "PRHS_Exception.h"

namespace PRHS {

	class Entity;
	class TextureManager;
	struct Texture;

	class Window {
		friend class TextureManager; //Allow the TextureManager class to access private members (primarily the renderer pointer)
		friend class Text; //Allow the Text class to access the renderer so that the Text class can manage its own textures
	public:

		static Window& getInstance(const int& windowWidth = 0, const int& windowHeight = 0, const bool& fullscreen = false);
		~Window();
		
		void setBackground(const std::string& textureManagerId); //Set the background image

		void render(const std::string& textureManagerId, const Rect& position); //Render a texture in a given area
		void render(const Entity& entity); //Render an entity
		void render(std::vector<Entity>& entityList); //Render all entities in a list

		void refreshBackground(); //Refresh entire background
		void refreshBackground(const Rect& area); //Refresh only a certain area
		void refreshBackground(const Entity& entity); //Refresh the background around an entity
		void refreshBackground(std::vector<Entity>& entityList); //Refresh the background around all entities in a list

		void updateDisplay();

		int getWidth();
		int getHeight();
	private:
		TextureManager* textureManager;

		//Enforce singleton status
		Window(const int windowWidth = 0, const int windowHeight = 0, const bool fullscreen = false);
		void operator=(const Window&) = delete;
		Window(const Window&) = delete;

		const double halfPI = 1.57079632679; //define PI for math calculations
		const int LEFT_UNDERSCAN = 50; //The number of pixels to ignore on the left side of the screen due to a flaw in the arcade machine construction
		bool backgroundSet; //Is TRUE if the background image has been set

		int simulatedWindowWidth; //Holds the width in pixels of the window
		int simulatedWindowHeight; //Holds the height in pixels of the window
		int realWindowWidth;
		int realWindowHeight;
		bool fullscreen; //Is TRUE if the window is in fullscreen mode

		SDL_Window* window; //Holds a pointer to the SDL_Window object used in the background
		SDL_Renderer* renderer; //Holds a pointer to the SDL_Renderer obect used in the background
		std::shared_ptr<Texture> background; //A smart pointer for the user-loaded texture used for the background

		SDL_Texture* black;
		SDL_Rect edge1; //Defines one edge in fullscreen mode
		SDL_Rect edge2; //Defines one edge in fullscreen mode
		SDL_Rect leftEdge; //Defines the left edge for compensating for screen coverage on the arcade machine

		float scaleFactor;
		int xOffset;
		int yOffset;

		SDL_Rect convertRect(const Rect& rect); //Converts a PRHS::Rect to an SDL_Rect
		SDL_Rect resizeRect(const Rect& rect); //Converts a PRHS::Rect to an SDL_Rect large enough to encompass the corners of a rotated PRHS::Rect
	};

};

