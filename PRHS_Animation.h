#pragma once

/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include <string>
#include <fstream>

#include "PRHS_Utils.h"
#include "PRHS_Exception.h"
#include "PRHS_TextureManager.h"
#include "PRHS_Timer.h"

namespace PRHS {

	class Animation {
	public:
		Animation();
		Animation(const std::string& pathToFile, const int updateTime); //Construct an animation from a file with the given update time (milliseconds between frames)

		//Overload copy constructor and operator= to ensure correct duplication
		Animation(const Animation& animationToCopy);
		void operator=(const Animation& animationToCopy);

		int size(); //Get the number of frames in the animation

		std::string next(); //Get the next frame in the animation
		std::string getFrame(const int position); //Get a certain frame

		int currentPosition(); //Returns the index of the next frame that will be returned by next() (First frame returns zero)
		void setFrame(const int newPosition); //Set the current frame. The given frame will be the next frame returned by next()
		bool atEnd();
		void reset();

		void setUpdateTime(const int newUpdateTime); //Set a new update time
		int getUpdateTime(); //Returns the current update time

		void append(const std::string& textureManagerId); //Append texture to animation
		void append(const Animation& animation); //Append animation to animation
		void operator+=(const std::string& textureManagerId); //Append texture to animation
		void operator+=(const Animation& animation); //Append animation to animation
		Animation operator+(const std::string& textureManagerId); //Add a texture to an animation
		Animation operator+(const Animation& animation); //Add an animation to an animation

		bool updateTimeElapsed();
	private:
		static TextureManager& textureManager;

		std::vector<std::string> animationIds; //Holds the IDs associated with the textures. These values are returned and passed between library functions as to preserve encapsulation of Textures
		std::vector<std::string>::iterator idIterator;

		std::vector<std::shared_ptr<Texture>> animation; //Holds shared pointers to the textures as to allow the TextureManager class to keep track of which textures are in use. Establishes usage of the textures so that they are not automatically deleted

		Timer updateTimer;
	};

}
