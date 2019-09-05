/*

Version 2.0.0 of the PRHS Game Library

Last Modified on January 18, 2019

*/

#include "PRHS_SoundManager.h"

namespace PRHS {

	SoundManager::SoundManager() {
		loadedMainTrack = NULL;
	}

	SoundManager& SoundManager::getInstance() {
		static SoundManager soundManager;
		return soundManager;
	}

	void SoundManager::setMainTrack(const std::string& pathToAudio) {
		
		std::ifstream inputFile(pathToAudio); //Attempt to open audio file
		bool notValidFile = !inputFile; //Store the validity of the file so that the file may be closed before throwing any exceptions
		inputFile.close();

		if (notValidFile) {
			throw invalid_path("No file found at \"" + pathToAudio + "\""); //Throw exception if file does not exist
		} else {
			if (loadedMainTrack != NULL) { //Check if a main track was already loaded
				Mix_FreeMusic(loadedMainTrack); //Free it if it was
			}

			loadedMainTrack = Mix_LoadMUS(pathToAudio.c_str()); //Load new main track
		}
	}

	void SoundManager::loadEffect(const std::string& pathToSoundEffect, const std::string& id) {

		std::ifstream inputFile(pathToSoundEffect); //Attempt to open audio file
		bool notValidFile = !inputFile; //Store the validity of the file so that the file may be closed before throwing any exceptions
		inputFile.close();

		if (notValidFile) {
			throw invalid_path("No file found at \"" + pathToSoundEffect + "\""); //Throw exception if file does not exist
		} else {
			if (hasId(id)) {
				throw invalid_id("Texture with id \"" + id + "\" already exists."); //Throw exception if a sound effect with the given id already exists
			} else {
				Mix_Chunk* loadedEffect = Mix_LoadWAV(pathToSoundEffect.c_str()); //Attempt to load effect

				if (loadedEffect == NULL) { //Check if sound effect was loaded correctly
					throw sdl_error("SDL error loading sound effect: " + std::string(Mix_GetError())); //Throw exception if it was not
				} else {
					soundMap.insert(std::pair<std::string, Mix_Chunk*>(id, loadedEffect)); //Add the effect to the map
				}
			}
		}
	}

	void SoundManager::mainTrack(const MainTrackCommand& command, const int& times) {
		switch (command) {
		case MAIN_TRACK_PLAY:
			Mix_PlayMusic(loadedMainTrack, times); //Play the main track the given number of times
			break;
		case MAIN_TRACK_PAUSE:
			Mix_PauseMusic(); //Pause the main track
			break;
		case MAIN_TRACK_RESUME:
			Mix_ResumeMusic(); //Resume the main track
			break;
		case MAIN_TRACK_STOP:
			Mix_HaltMusic(); //Stop the main track
			break;
		}
	}

	void SoundManager::setMainTrackVolume(const int& newVolume) {
		if (newVolume > 100) { //Account for the rebels who put in an invalid value above 100
			Mix_VolumeMusic(MIX_MAX_VOLUME);
		}
		else if (newVolume < 0) { //Account for the sarcastic assholes who put in a negative value for volume
			Mix_VolumeMusic(0);
		}
		else { //Deal with the unimaginative regular people who use the library like they're supposed to
			Mix_VolumeMusic(newVolume * (MIX_MAX_VOLUME / 100));
		}
	}

	void SoundManager::playEffect(const std::string& soundManagerId, const int& times) {
		if (hasId(soundManagerId)) { //Check if the id is valid
			if (times == -1) {
				Mix_PlayChannel(-1, soundMap[soundManagerId], -1); //Play the effect an infinite number of times
			} else if (times != 0) { //If zero, do nothing
				Mix_PlayChannel(-1, soundMap[soundManagerId], times - 1); //Play the effect the specified number of times
			}
		} else {
			throw invalid_id("No sound effect with id \"" + soundManagerId + "\" exists."); //Throw exception if id is not valid
		}
	}

	bool SoundManager::hasId(const std::string& id) {
		return soundMap.count(id);
	}

	void SoundManager::destroyEffect(const std::string& id) {
		if (hasId(id)) {
			Mix_FreeChunk(soundMap[id]); //Free the Mix_Chunk
			soundMap.erase(id); //Erase the sound effect from the list
		} else {
			throw invalid_id("No sound effect with id \"" + id + "\" exists."); //Throw exception if id is not valid
		}
	}
}