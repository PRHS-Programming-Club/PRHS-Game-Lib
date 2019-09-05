#pragma once

/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include "PRHS_Utils.h"

namespace PRHS {

	enum TimerState {
		TIMER_RUNNING, TIMER_STOPPED, TIMER_RESET
	};

	class Timer {
	public:
		Timer(const int& duration = 0);

		void startTimer();
		void stopTimer();
		void resetTimer();

		bool timeElapsed() const;
		int getTimeElapsed() const;

		int getDuration() const;
		void setDuration(const int& newDuration);

		TimerState getState() const;

		static int currentTime();
	private:
		int timerDuration;
		int timeStarted;
		int previousTimeElapsed;
		TimerState state;
	};

}


