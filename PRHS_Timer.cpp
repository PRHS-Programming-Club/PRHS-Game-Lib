/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include "stdafx.h"
#include "PRHS_Timer.h"

namespace PRHS {

	Timer::Timer(const int& duration) {
		timerDuration = duration;
		timeStarted = 0;
		previousTimeElapsed = 0;
		state = TIMER_STOPPED;
	}

	void Timer::startTimer() {
		if (state != TIMER_RUNNING) {
			timeStarted = SDL_GetTicks();
			state = TIMER_RUNNING;
		}
	}

	void Timer::stopTimer() {
		if (state == TIMER_RUNNING) {
			previousTimeElapsed += SDL_GetTicks() - timeStarted;
			state = TIMER_STOPPED;
		}
	}

	void Timer::resetTimer() {
		previousTimeElapsed = 0;
		state = TIMER_RESET;
	}

	bool Timer::timeElapsed() {
		return (previousTimeElapsed + SDL_GetTicks() - timeStarted) >= timerDuration;
	}

	int Timer::getTimeElapsed() {
		return previousTimeElapsed + SDL_GetTicks() - timeStarted;
	}

	int Timer::getDuration() {
		return timerDuration;
	}

	void Timer::setDuration(const int& newDuration) {
		timerDuration = newDuration;
	}

	TimerState Timer::getState() {
		return state;
	}

	int Timer::currentTime() {
		return SDL_GetTicks();
	}
}