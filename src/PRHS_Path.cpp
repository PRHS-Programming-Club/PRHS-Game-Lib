/*

Version 2.0.0 of the PRHS Game Library

Last Modified on January 18, 2019

*/

#include "PRHS_Path.h"

namespace PRHS {

	Path::Path() {
		deltaX = deltaY = deltaW = deltaH = deltaR = 0; //Initialize delta values

		//Initialize other member variables to default values
		updateParam = UPDATE_RELATIVE;
		updateTimer = Timer();

		iterator = path.begin(); //Set iterator
	}

	Path::Path(const EntityUpdateParam& updateParam, const int& updateTime) {
		deltaX = deltaY = deltaW = deltaH = deltaR = 0; //Initialize delta values

		//Initialize member variables based on constructor input
		this->updateParam = updateParam;
		updateTimer = Timer(updateTime);

		iterator = path.begin(); //Set iterator
	}

	Path::Path(const std::string& pathToFile, const EntityUpdateParam& updateParam, const int& updateTime) {

		std::ifstream inputFile(pathToFile, std::ios::in); //Attempt to open input file

		if (!inputFile) { //Check if file was successfully opened
			inputFile.close();
			throw invalid_path("File at " + pathToFile + " could not be opened or does not exist."); //Throw error if file was not correctly opened
		}
		else { //Continue reading file
			Rect tempRect;
			while (!inputFile.eof()) { //Read file
				//Read data from file
				inputFile >> tempRect.x;
				inputFile >> tempRect.y;
				inputFile >> tempRect.w;
				inputFile >> tempRect.h;
				inputFile >> tempRect.r;

				//If a relative path, keep track of changes
				if (updateParam == UPDATE_RELATIVE) {
					deltaX += tempRect.x;
					deltaY += tempRect.y;
					deltaW += tempRect.w;
					deltaH += tempRect.h;
					deltaR += tempRect.r;
				}

				path.push_back(tempRect); //Add rect to path list
			}

			inputFile.close();

			//Initialize member variables based on constructor input
			this->updateParam = updateParam;
			updateTimer = Timer(updateTime);

			iterator = path.begin(); //Set iterator
		}
	}

	int Path::determineStep(int &numLongSteps, const int& shortStep, const int& delta) {
		if (numLongSteps != 0) {
			numLongSteps--;
			if (shortStep > 0) {
				return shortStep + 1;
			}
			else if (shortStep == 0) {
				if (delta < 0) {
					return shortStep - 1;
				}
				if (delta > 0) {
					return shortStep + 1;
				}
				return 0;
			}
			else {
				return shortStep - 1;
			}
		}
		else {
			return shortStep;
		}
	}

	Path::Path(const Rect& origin, const Rect& destination, const int& steps, const int& updateTime) {

		deltaX = destination.x - origin.x;
		deltaY = destination.y - origin.y;
		deltaW = destination.w - origin.w;
		deltaH = destination.h - origin.h;
		deltaR = destination.r - origin.r;

		const int shortXStep = deltaX / steps;
		int numXLongSteps = abs(deltaX % steps);
		const int shortYStep = deltaY / steps;
		int numYLongSteps = abs(deltaY % steps);
		const int shortWStep = deltaW / steps;
		int numWLongSteps = abs(deltaW % steps);
		const int shortHStep = deltaH / steps;
		int numHLongSteps = abs(deltaH % steps);
		const int shortRStep = deltaR / steps;
		int numRLongSteps = abs(deltaR % steps);

		path.reserve(steps); //Reserve space for all elements

		int numSteps = steps;
		while (numSteps != 0) {
			Rect tempRect;

			//Calculate step sizes
			tempRect.x = determineStep(numXLongSteps, shortXStep, deltaX);
			tempRect.y = determineStep(numYLongSteps, shortYStep, deltaY);
			tempRect.w = determineStep(numWLongSteps, shortWStep, deltaW);
			tempRect.h = determineStep(numHLongSteps, shortHStep, deltaH);
			tempRect.r = determineStep(numRLongSteps, shortRStep, deltaR);

			//Keep track of changes
			deltaX += tempRect.x;
			deltaY += tempRect.y;
			deltaW += tempRect.w;
			deltaH += tempRect.h;
			deltaR += tempRect.r;

			path.push_back(tempRect);
			numSteps--;
		}

		//Initialize member variables based on constructor input
		updateParam = UPDATE_RELATIVE;
		updateTimer = Timer(updateTime);

		iterator = path.begin(); //Set iterator
	}

	Path::Path(const Path& pathToCopy) {
		path = pathToCopy.path; //Copy vector
		iterator = path.begin(); //Correctly set iterator

		//Copy all parameter member variables
		updateParam = pathToCopy.updateParam;
		updateTimer = pathToCopy.updateTimer;

		//Copy delta values
		deltaX = pathToCopy.deltaX;
		deltaY = pathToCopy.deltaY;
		deltaW = pathToCopy.deltaW;
		deltaH = pathToCopy.deltaH;
		deltaR = pathToCopy.deltaR;
	}

	void Path::operator=(const Path& pathToCopy) {
		path = pathToCopy.path; //Copy vector
		iterator = path.begin(); //Correctly set iterator

		//Copy all parameter member variables
		updateParam = pathToCopy.updateParam;
		updateTimer = pathToCopy.updateTimer;

		//Copy delta values
		deltaX = pathToCopy.deltaX;
		deltaY = pathToCopy.deltaY;
		deltaW = pathToCopy.deltaW;
		deltaH = pathToCopy.deltaH;
		deltaR = pathToCopy.deltaR;
	}

	int Path::size() {
		return path.size();
	}

	Rect Path::next() {
		updateTimer.resetTimer();
		updateTimer.startTimer();
		if (atEnd()) { //Check if at end of the path
			iterator = path.begin(); //Handle wrapping
			iterator++;
			return *path.begin();
		}
		iterator++; //Increment iterator
		return *std::prev(iterator, 1);
	}

	Rect Path::getFrame(const int& frameIndex) {
		return path.at(frameIndex);
	}

	int Path::currentPosition() {
		return iterator - path.begin(); //Get difference between current position and the beginning of the vector
	}

	void Path::setFrame(const int& newPosition) {
		iterator = path.begin() + newPosition;
	}

	bool Path::atEnd() {
		return iterator == path.end(); //Check if the iterator is at the end
	}

	void Path::reset() {
		iterator = path.begin(); //Reset the iterator to the beginning
		updateTimer.resetTimer(); //Reset the timer
	}

	int Path::getDeltaX() {
		return deltaX;
	}

	int Path::getDeltaY() {
		return deltaY;
	}

	int Path::getDeltaW() {
		return deltaW;
	}

	int Path::getDeltaH() {
		return deltaH;
	}

	int Path::getDeltaR() {
		return deltaR;
	}

	void Path::setUpdateTime(const int& newUpdateTime) {
		updateTimer.setDuration(newUpdateTime);
	}

	int Path::getUpdateTime() {
		return updateTimer.getDuration();
	}

	void Path::setUpdateParam(const EntityUpdateParam& newUpdateParam) {
		updateParam = newUpdateParam;
	}

	EntityUpdateParam Path::getUpdateParam() {
		return updateParam;
	}

	void Path::append(Path& path) {

		if (updateParam != path.updateParam) { //Check if updateParameters are the same
			throw parameter_mismatch("Path update parameters do not match."); //Throw exception if they are not (Cannot combine absolute and relative paths)
		}

		this->path.reserve(this->path.size() + path.size()); //Reserve enough space for the new elements
		this->path.insert(this->path.end(), path.path.begin(), path.path.end()); //Copy the elements into the vector
		reset();
	}

	void Path::append(Rect& rect) {
		this->path.push_back(rect); //Append the rect to the end of the vector
		reset();
	}

	void Path::operator+=(Path& path) {
		append(path);
	}

	void Path::operator+=(Rect& rect) {
		append(rect);
	}

	Path Path::operator+(Path& path) {
		Path output = *this; //Copy the current path
		output.append(path); //Append the given path
		return output;
	}

	Path Path::operator+(Rect& rect) {
		Path output = *this; //Copy the current path
		output.append(rect); //Append the given rect
		return output;
	}

	bool Path::updateTimeElapsed() {
		return updateTimer.timeElapsed(); //Refer to Timer::timeElapsed()
	}
}
