#pragma once

/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include <fstream>
#include <map>
#include <memory>

#include "PRHS_Utils.h"

namespace PRHS {

	enum MainTrackCommand {
		MAIN_TRACK_PLAY, MAIN_TRACK_PAUSE, MAIN_TRACK_RESUME, MAIN_TRACK_STOP
	};

	class SoundManager {
	public:

		static SoundManager& getInstance(); //Returns the singleton instance of the SoundManager
		
		void setMainTrack(const std::string& pathToAudio); //Sets the main track and automatically frees the previous main track if it exists
		void loadEffect(const std::string& pathToSoundEffect, const std::string& id); //Loads an effect and assigns it the given ID. Throws invalid_id if an effect with the same ID already exists

		void mainTrack(const MainTrackCommand& command, const int& times = 1); //Allows the user to execute and MainTrackCommand. The "loops" parameter is optional and is only used if the MAIN_TRACK_PLAY command is entered
		void setMainTrackVolume(const int& newVolume); //Sets the volume of the main track on a scale of 0 to 100

		void playEffect(const std::string& id, const int& times = 1); //Allows the user to play a sound effect with the given ID. If no sound effect with the given ID is present, this function throws an invalid_id exception

		bool hasId(const std::string& id); //Returns true if an effect with the given ID exists

		void destroyEffect(const std::string& id); //Destroys the effect with the given ID, freeing its memory
	private:
		static SoundManager& singletonSoundManager;

		//Enforce singleton status
		SoundManager();
		SoundManager(const SoundManager&) = delete;
		void operator=(const SoundManager&) = delete;

		Mix_Music* loadedMainTrack;
		std::map<std::string, Mix_Chunk*> soundMap;
	};

}

