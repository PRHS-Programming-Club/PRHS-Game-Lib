#pragma once

/*

Version 2.0.0 of the PRHS Game Library

Last Modified on August 5th, 2017

*/

#include <string>

#include "PRHS_Utils.h"
#include "PRHS_TextureManager.h"

namespace PRHS {

	enum EntityUpdateParam {
		UPDATE_RELATIVE, UPDATE_ABSOLUTE
	};

	class Entity {
		friend class Text; //Allow the Text class to access the Entity's texture directly so that it can manage its own resources more efficiently than using the texture manager
		friend class Window; //Allow the window class to access the Entity's texture directly which is more efficient than going through the texture manager
	public:
		Entity(); //Empty entity constructor
		Entity(const std::string& textureManagerId, const FloatRect& newPosition); //Constructs an entity with the given texture and position

		void updatePosition(const FloatRect& adjustment, const EntityUpdateParam& updateParam);
		void setPosition(int newX, int newY, int newW, int newH, int newR);
		FloatRect getPosition();

		bool checkCollision(const FloatRect& rect);
		bool checkCollision(const Entity& entity);
		bool onScreen();

		void setSkin(const std::string& textureManagerId);
		std::string getSkinId();

	protected:
		FloatRect position;
		std::string skinId;
		static TextureManager& textureManager; //Store a reference to the TextureManager

	private:
		std::shared_ptr<Texture> skin; //The shared pointer allows the TextureManager class to keep track of which textures are currently in use
	};
};