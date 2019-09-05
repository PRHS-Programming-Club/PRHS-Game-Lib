#pragma once

/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include <string>
#include <vector>
#include <fstream>

#include "PRHS_Utils.h"
#include "PRHS_Entity.h"
#include "PRHS_Exception.h"
#include "PRHS_Timer.h"

namespace PRHS {

	class Path {
	public:
		Path();
		Path(const EntityUpdateParam& updateParam, const int& updateTime);
		Path(const std::string& pathToFile, const EntityUpdateParam& updateParam, const int& updateTime);
		Path(const Rect& origin, const Rect& destination, const int& steps, const int& updateTime);

		Path(const Path& pathToCopy);
		void operator=(const Path& pathToCopy);

		int size();

		Rect next();
		Rect getFrame(const int& frameIndex);

		int currentPosition(); //Returns the index of the next frame that will be returned by next() (First frame returns zero)
		void setFrame(const int& newPosition); //Set the current frame. The given frame will be the next frame returned by next()
		bool atEnd();
		void reset();

		int getDeltaX();
		int getDeltaY();
		int getDeltaW();
		int getDeltaH();
		int getDeltaR();

		void setUpdateTime(const int& newUpdateTime);
		int getUpdateTime();
		void setUpdateParam(const EntityUpdateParam& newUpdateParam);
		EntityUpdateParam getUpdateParam();

		void append(Path& path);
		void append(Rect& rect);
		void operator+=(Path& path);
		void operator+=(Rect& rect);
		Path operator+(Path& path);
		Path operator+(Rect& rect);

		bool updateTimeElapsed();
	private:
		std::vector<Rect> path; //Holds a list of PRHS_Rects that define the path.
		std::vector<Rect>::iterator iterator; //Keeps track of which PRHS_Rect was last returned.

		EntityUpdateParam updateParam; //Holds whether the path is an absolute path or a relative path.
		Timer updateTimer; //Handles update time checking

		int deltaX; //Holds the total change in x-position over the total path
		int deltaY; //Holds the total change in y-position over the total path
		int deltaW; //Holds the total change in width over the total path
		int deltaH; //Holds the total change in height over the total path
		int deltaR; //Holds the total change in rotation over the total path

		int determineStep(int& numLongSteps, const int& shortStep, const int& delta);
	};
}