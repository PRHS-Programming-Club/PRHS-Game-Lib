/*

Version 2.0.0 of the PRHS Game Library

Last Modified on January 18, 2019

*/

#include "PRHS_Text.h"

namespace PRHS {

	Text::FontManager& Text::fontManager = Text::FontManager::getInstance();

	Text::FontManager::FontManager() {

	}

	Text::FontManager& Text::FontManager::getInstance() {
		static Text::FontManager fontManager;
		return fontManager;
	}

	bool Text::FontManager::loadFont(const std::string& pathToFont, const int& fontSize) {
		std::string id = pathToFont + std::to_string(fontSize); //Generate font manager id from path and size
		if (!hasId(id)) { //Only load the font if it does not already exist
			std::ifstream fileTest(pathToFont); //Attempt to open the file
			bool notValidFile = !fileTest; //Store the validity of the file so that the file may be closed before throwing any exceptions
			fileTest.close(); //Close the file

			if (notValidFile) {
				throw invalid_path(pathToFont + " is not a valid path.");
			}
			else {
				TTF_Font* font = TTF_OpenFont(pathToFont.c_str(), fontSize); //Attempt to load font
				if (font == NULL) { //If the font was not successfully loaded
					throw sdl_error("SDL error creating font: " + std::string(SDL_GetError())); //Throw exception
				}
				fontMap.insert(std::pair<std::string, std::shared_ptr<TTF_Font*>>(id, std::make_shared<TTF_Font*>(font))); //Add the shared pointer to the map with the given ID
				fontsLoaded++; //Increment font loader counter

				if (fontsLoaded >= 10) {
					freeUnused(); //Automatically free unused fonts after every 10 fonts that are loaded
				}
			}
		}
		else {
			throw invalid_id("A font with the name \"" + id + "\" already exists.");
		}
	}

	std::shared_ptr<TTF_Font*> Text::FontManager::getFont(const std::string& id) {
		if (hasId(id)) { //Check if valid font
			return fontMap[id];
		}
		else {
			throw invalid_id("No font with name \"" + id + "\" exists.");
		}
	}

	bool Text::FontManager::hasId(const std::string& id) {
		return fontMap.count(id);
	}

	bool Text::FontManager::destroyFont(const std::string& id) {
		if (hasId(id)) { //Check if valid id
			if (fontMap[id].unique()) { //Check if the font is being used
				TTF_CloseFont(*fontMap[id]); //Free the font
				fontMap.erase(id); //Remove the shared pointer from the map
				return true; //Indicate that the font was destroyed
			}
			else {
				return false; //Indicate that the font was not destroyed
			}
		}
		else {
			throw invalid_id("No font with name \"" + id + "\" exists.");
		}
	}

	void Text::FontManager::freeUnused() {
		for (std::map<std::string, std::shared_ptr<TTF_Font*>>::iterator i = fontMap.begin(); i != fontMap.end(); i++) { //Iterate through all fonts
			if ((*i).second.unique()) { //Check if the font is being used
				TTF_CloseFont(*(*i).second); //Free the font
				fontMap.erase(i); //Remove the shared pointer from the map
			}
		}
		fontsLoaded = 0;
	}

	Text::Text(const std::string& pathToFont, const int& fontSize) {
		fontManager.loadFont(pathToFont, fontSize); //Load the font into the fontManager
		fontManagerId = pathToFont + std::to_string(fontSize); //Generate the fontManagerId
		font = fontManager.getFont(fontManagerId); //Get a shared pointer from the font manager
	}

	Text::Text(const std::string& text, const std::string& pathToFont, const int& fontSize, const Color& textColor, const int& xPosition, const int& yPosition) {
		fontManager.loadFont(pathToFont, fontSize); //Load the font into the fontManager
		fontManagerId = pathToFont + std::to_string(fontSize); //Generate the fontManagerId
		font = fontManager.getFont(fontManagerId); //Get a shared pointer from the font manager

		this->text = text;
		this->fontSize = fontSize;
		this->textColor = textColor;

		//Set position
		position.x = xPosition;
		position.y = yPosition;

		updateTexture(); //Update the texture to reflect changes
	}

	Text::Text(const float& number, const std::string& pathToFont, const int& fontSize, const Color& textColor, const int& xPosition, const int& yPosition) {
		fontManager.loadFont(pathToFont, fontSize); //Load the font into the fontManager
		fontManagerId = pathToFont + std::to_string(fontSize); //Generate the fontManagerId
		font = fontManager.getFont(fontManagerId); //Get a shared pointer from the font manager

		this->text = std::to_string(number); //Convert the number to a string
		this->fontSize = fontSize;
		this->textColor = textColor;

		//Set position
		position.x = xPosition;
		position.y = yPosition;

		updateTexture(); //Update the texture to reflect changes
	}

	Text::~Text() {
		if (skin.unique()) { //Check if any other copies of the texture are in use (possibly by copies of the texture in vectors or function parameters). This ensures that the texture remains valid until the last Text object using it is destroyed
			SDL_DestroyTexture((*skin).texture); //Free the texture
		}
	}

	void Text::setText(const std::string& newText) {
		text = newText;
		updateTexture();
	}

	void Text::setText(const float& newText) {
		text = std::to_string(newText);
		updateTexture();
	}

	void Text::setColor(const Color& newColor) {
		textColor = newColor;
		updateTexture();
	}

	void Text::append(const std::string& newText) {
		text.append(newText);
		updateTexture();
	}

	void Text::append(const float& newText) {
		text.append(std::to_string(newText));
		updateTexture();
	}

	void Text::operator+=(const std::string& newText) {
		append(newText);
	}

	void Text::operator+=(const float& newText) {
		append(newText);
	}

	Text Text::operator+(const std::string& newText) {
		Text output = *this; //Create a copy of the current text object to build off of
		output.append(newText); //Append the new text
		return output;
	}

	Text Text::operator+(const float& newText) {
		Text output = *this; //Create a copy of the current text object to build off of
		output.append(std::to_string(newText)); //Append the number converted to a string
		return output;
	}

	std::string Text::getText() {
		return text;
	}

	void Text::updateTexture() {
		if (skin.unique()) { //Free the texture if it is not in use by anything else
			SDL_DestroyTexture((*skin).texture);
		}

		SDL_Surface* tempSurface = TTF_RenderText_Solid((*font), text.c_str(), textColor); //Create an SDL_Surface using the parameters defined by the text object
		skin = std::make_shared<Texture>(Texture(SDL_CreateTextureFromSurface(Window::getInstance().renderer, tempSurface), "NO_PATH")); //Create an SDL_Texture and store it in a smart pointer
		SDL_FreeSurface(tempSurface); //Free the temporary surface used to create the texture

		//Calculate the new width and height of the entity
		TTF_SizeText((*font), text.c_str(), &position.w, &position.h);
	}
}