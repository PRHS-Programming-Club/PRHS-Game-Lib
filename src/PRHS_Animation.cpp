/*

Version 2.0.0 of the PRHS Game Library

Last Modified on January 18, 2019

*/

#include "PRHS_Animation.h"
#include <algorithm>

namespace PRHS {

	TextureManager& Animation::textureManager = TextureManager::getInstance();

	Animation::Animation() {

	}

	Animation::Animation(const std::string& pathToFile, const int updateTime) {
		std::ifstream inputFile(pathToFile, std::ios::in); //Attempt to open file

		if (!inputFile) { //Check if file exists
			inputFile.close();
			throw invalid_path("File at " + pathToFile + " could not be opened or does not exist."); //Throw exception if file does not exist
		}
		else { //Read file
			std::string line;
			while (!inputFile.eof()) {
				std::getline(inputFile, line); //Read next line
				if (line == "") {
					continue;
				}

				if (!textureManager.hasId(line)) { //Check if image was already loaded based on its path. The image's path is used as its ID
					textureManager.createTexture(line, line); //If it was not already loaded, load the image. The TextureManager class will check if it is a valid file
				}

				animationIds.push_back(line); //Store the textureId for the animation
				animation.push_back(textureManager.getTexture(line)); //Add the texture to the animation
			}
			inputFile.close();
		}

		idIterator = animationIds.begin();

		updateTimer = Timer(updateTime);
	}

	Animation::Animation(const Animation& other) :
		animation(other.animation),
		animationIds(other.animationIds),
		idIterator(animationIds.begin() + other.currentPosition()),
		updateTimer(other.updateTimer)
	{

	}

	Animation::Animation(Animation&& other)
		: Animation()
	{
		swap(*this, other);
	}

	Animation& Animation::operator=(Animation other) {
		swap(*this, other);
		return *this;
	}

	int Animation::size() const {
		return animationIds.size();
	}

	std::string Animation::next() {
		updateTimer.resetTimer();
		updateTimer.startTimer();
		if (atEnd()) { //Check if at end of the animation
			idIterator = animationIds.begin() + 1; //Handle wrapping
			return *animationIds.begin();
		}
		return *idIterator++; //Increment iterator
	}

	std::string Animation::getFrame(const int frameIndex) {
		return animationIds.at(frameIndex);
	}

	int Animation::currentPosition() const {
		return idIterator - animationIds.begin(); //Get difference between current position and the beginning of the vector
	}

	void Animation::setFrame(const int newPosition) {
		idIterator = animationIds.begin() + newPosition;
	}

	bool Animation::atEnd() const {
		return idIterator == animationIds.end(); //Check if the iterator is at the end
	}

	void Animation::reset() {
		idIterator = animationIds.begin(); //Reset the iterator to the beginning
		updateTimer.resetTimer(); //Reset the timer
	}

	void Animation::setUpdateTime(const int newUpdateTime) {
		updateTimer.setDuration(newUpdateTime);
	}

	int Animation::getUpdateTime() const {
		return updateTimer.getDuration();
	}

	void Animation::append(const std::string& textureManagerId) {
		if (textureManager.hasId(textureManagerId)) { //Check if the texture exists
			animationIds.push_back(textureManagerId); //If it does, add it to the animation
			animation.push_back(textureManager.getTexture(textureManagerId)); //Get a shared pointer to the texture to establish usage
			idIterator = animationIds.begin();
		}
		else {
			throw invalid_id("Texture with id \"" + textureManagerId + "\" does not exist."); //Throw exception if the texture does not exist
		}
	}

	void Animation::append(const Animation& animation) {
		this->animationIds.insert(this->animationIds.begin(), animation.animationIds.begin(), animation.animationIds.end()); //Append the new animationIds vector to the old one
		this->animation.insert(this->animation.begin(), animation.animation.begin(), animation.animation.end()); //Append the new animation vector to the old one
		idIterator = animationIds.begin();
	}

	void Animation::operator+=(const std::string& textureManagerId) {
		append(textureManagerId);
	}

	void Animation::operator+=(const Animation& animation) {
		append(animation);
	}

	Animation Animation::operator+(const std::string& textureManagerId) {
		Animation output = *this; //Copy the current animation
		output.append(textureManagerId); //Append the given texture
		return output;
	}

	Animation Animation::operator+(const Animation& animation) {
		Animation output = *this; //Copy the current animation
		output.append(animation); //Append the given animation
		return output;
	}

	bool Animation::updateTimeElapsed() const {
		return updateTimer.timeElapsed(); //Refer to Timer::timeElapsed()
	}
	
	void swap(Animation& lhs, Animation& rhs) {
		using std::swap;
		swap(lhs.animationIds, rhs.animationIds);
		swap(lhs.animation, rhs.animation);
		swap(lhs.idIterator, rhs.idIterator);
		swap(lhs.updateTimer, rhs.updateTimer);
	}
}