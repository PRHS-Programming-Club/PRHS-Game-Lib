/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include "stdafx.h"
#include "PRHS_Exception.h"

namespace PRHS {

	invalid_path::invalid_path(const std::string& errorString) {
		this->errorString = errorString;
	}

	const char* invalid_path::what() {
		return errorString.c_str();
	}

	invalid_id::invalid_id(const std::string& errorString) {
		this->errorString = errorString;
	}

	const char* invalid_id::what() {
		return errorString.c_str();
	}

	parameter_mismatch::parameter_mismatch(const std::string& errorString) {
		this->errorString = errorString;
	}

	const char* parameter_mismatch::what() {
		return errorString.c_str();
	}

	library_not_initialized::library_not_initialized(const std::string& errorString) {
		this->errorString = errorString;
	}

	const char* library_not_initialized::what() {
		return errorString.c_str();
	}

	sdl_error::sdl_error(const std::string& errorString) {
		this->errorString = errorString;
	}

	const char* sdl_error::what() {
		return errorString.c_str();
	}
}