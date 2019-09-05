#pragma once

/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include <string>
#include <map>

#include "PRHS_Utils.h"
#include "PRHS_Entity.h"
#include "PRHS_TextureManager.h"
	
namespace PRHS {
	
	typedef SDL_Color Color; //Define PRHS::Color to be equivalent to SDL_Color

	class Text : public Entity {
	public:

		Text(const std::string& pathToFont, const int& fontSize);
		Text(const std::string& text, const std::string& pathToFont, const int& fontSize, const Color& textColor, const int& xPosition, const int& yPosition);
		Text(const float& number, const std::string& pathToFont, const int& fontSize, const Color& textColor, const int& xPosition, const int& yPosition);
		~Text();

		void setText(const std::string& newText);
		void setText(const float& number);

		void setColor(const Color& newColor);

		void append(const std::string& text);
		void append(const float& number);
		void operator+=(const std::string& text);
		void operator+=(const float& number);
		Text operator+(const std::string& text);
		Text operator+(const float& number);

		std::string getText() const;
	private:

		class FontManager {
		public:

			static FontManager& getInstance();

			bool loadFont(const std::string& pathToFont, const int& fontSize);
			std::shared_ptr<TTF_Font*> getFont(const std::string& id); //FontManager ids consist of fontPath + fontSize (e.g. Assets/myFont.ttf12)

			bool hasId(const std::string& id);
		private:
			//Enforce singleton status
			FontManager();
			FontManager(const FontManager&) = delete;
			void operator=(const FontManager&) = delete;

			std::map<std::string, std::shared_ptr<TTF_Font*>> fontMap;

			int fontsLoaded; //Stores the number of fonts loaded since last call of freeUnused()

			bool destroyFont(const std::string& id);
			void freeUnused(); //Destroys all fonts that are not currently in use. This function is called automatically every tenth time a new font is loaded
		};

		static FontManager& fontManager; //FontManager reference used by Text class

		std::string text;
		int fontSize;
		Color textColor;

		std::string fontManagerId; //The id used to refer to the font within the fontManager
		std::shared_ptr<TTF_Font*> font; //The font used by the texture
		
		void updateTexture(); //This method is called whenever the Text object is changed. Text textures are stored within the textureManager and given unique numerical ids (e.g. text5419). When the texture is updated, the previous texture is destroyed
	};
}