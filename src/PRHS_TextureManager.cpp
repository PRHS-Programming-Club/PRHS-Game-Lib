/*

Version 2.0.0 of the PRHS Game Library

Last Modified on January 18, 2019

*/

#include "PRHS_TextureManager.h"

namespace PRHS {

	Texture::Texture(SDL_Texture* texture, std::string pathToImage) {
		this->texture = texture;
		this->imagePath = pathToImage;
	}

	TextureManager::TextureManager() {

	}

	TextureManager& TextureManager::getInstance() {
		static TextureManager textureManager;
		return textureManager;
	}

	void TextureManager::createTexture(const std::string& pathToImage, const std::string& id) {

		if (textureMap.count(id)) { //Check if a texture with the given id already exists
			throw invalid_id("Texture with id \"" + id + "\" already exists."); //Throw exception
		} else {
			SDL_Renderer* renderer = Window::getInstance().renderer; //Get a pointer to the renderer

			std::ifstream fileTest(pathToImage); //Attempt to open the file
			bool notValidFile = !fileTest; //Store the validity of the file so that the file may be closed before throwing any exceptions
			fileTest.close(); //Close the file

			if (notValidFile) { //Check if valid path
				throw invalid_path(pathToImage + " is not a valid path.");
			}
			else if (!renderer) { //Check if valid renderer
				throw library_not_initialized("A valid renderer was not found when attempting to create a texture.");
			}
			else {
				SDL_Surface* tempSurface = IMG_Load(pathToImage.c_str()); //Load image

				if (!tempSurface) { //Check if surface was created correctly
					throw sdl_error("SDL error creating surface: " + std::string(SDL_GetError())); //Throw exception
				}
				else { //Attempt to create texture
					SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
					SDL_FreeSurface(tempSurface);

					if (!texture) { //Check if texture was created correctly
						throw sdl_error("SDL error creating texture: " + std::string(SDL_GetError())); //Trow exception
					}
					else { //Add texture to map
						textureMap.insert(std::pair<std::string, std::shared_ptr<Texture>>(id, std::make_shared<Texture>(Texture(texture, pathToImage))));
					}
				}
			}
		}
	}

	std::shared_ptr<Texture> TextureManager::getTexture(const std::string id) {

		if (textureMap.count(id)) { //Check if an element with the desired id exists
			return textureMap[id];
		}
		else {
			throw invalid_id("No texture found with id \"" + id + "\"."); //Throw exception
		}
	}

	bool TextureManager::hasId(const std::string& id) {
		return textureMap.count(id);
	}

	bool TextureManager::destroyTexture(const std::string& id) {
		if (textureMap.count(id)) { //Check if an element with the desired id exists
			if (textureMap[id].unique()) { //Check if the texture is being used anywhere else
				SDL_DestroyTexture((*textureMap[id]).texture); //Destroy the texture
				textureMap.erase(id); //Remove the texture from the list
				return true; //Indicate that the texture was destroyed
			}
			else {
				return false; //Indicate that the texture could not be destroyed
			}
		}
		else {
			throw invalid_id("No texture found with id \"" + id + "\"."); //Throw exception if texture does not exist
		}
	}

	void TextureManager::freeUnused() {
		for (std::map<std::string, std::shared_ptr<Texture>>::iterator iterator = textureMap.begin(); iterator != textureMap.end(); iterator++) { //Iterate through all textures
			if ((*iterator).second.unique()) { //Check if texture is being used anywhere else
				SDL_DestroyTexture((*(*iterator).second).texture); //Destroy the texture
				textureMap.erase(iterator); //Remove entry from map
			}
		}
	}
}