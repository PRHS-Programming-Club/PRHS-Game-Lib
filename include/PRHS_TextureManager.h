#pragma once

/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include <map>
#include <memory>
#include <string>
#include <fstream>

#include "PRHS_Utils.h"
#include "PRHS_Exception.h"
#include "PRHS_Window.h"

namespace PRHS {
	
	struct Texture {
		friend class Window;
		friend class TextureManager;
		friend class Text;
	private: 
		SDL_Texture* texture;
		std::string imagePath;
		Texture(SDL_Texture* texture, std::string pathToImage);
	};

	class TextureManager {
		friend class Window;
		friend class Entity;
		friend class Animation;
	public:

		static TextureManager& getInstance();

		void createTexture(const std::string& pathToImage, const std::string& id); //Returns false if invalid path or if the desired id is already in use

		bool hasId(const std::string& id); //Returns true if the texture manager contains a texture with the given id

		bool destroyTexture(const std::string& id); //Checks if the texture has any remaining references and destroys it if it does not
		void freeUnused(); //Destroys all textures that are not currently in use

	private:

		//Enforce singleton status
		TextureManager();
		TextureManager(const TextureManager&) = delete;
		void operator=(const TextureManager&) = delete;

		std::shared_ptr<Texture> getTexture(const std::string id);

		std::map<std::string, std::shared_ptr<Texture>> textureMap; //Map to hold user-loaded texture values
	};

};


