#pragma once

/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include <exception>
#include <string>

namespace PRHS {

	class invalid_path : public std::exception {
	public:
		invalid_path(const std::string& errorString);
		const char* what();
	private:
		std::string errorString;
	};



	class invalid_id : public std::exception {
	public:
		invalid_id(const std::string& errorString);
		const char* what();
	private:
		std::string errorString;
	};



	class parameter_mismatch : public std::exception {
	public:
		parameter_mismatch(const std::string& errorString);
		const char* what();
	private:
		std::string errorString;
	};



	class library_not_initialized : public std::exception {
	public:
		library_not_initialized(const std::string& errorString);
		const char* what();
	private:
		std::string errorString;
	};



	class sdl_error : public std::exception {
	public:
		sdl_error(const std::string& errorString);
		const char* what();
	private:
		std::string errorString;
	};

};

