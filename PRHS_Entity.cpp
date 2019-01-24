/*

Version 2.0.0 of the PRHS Game Library

Last Modified on January 18, 2019

*/

#include "PRHS_Entity.h"
#include "PRHS_Window.h"

namespace PRHS {

	TextureManager& Entity::textureManager = TextureManager::getInstance();

	Entity::Entity() :
		skinId("NO SKIN"),
		position{ 0.0, 0.0, 0.0, 0.0, 0.0 },
		skin()
	{

	}

	Entity::Entity(const std::string& textureManagerId, const FloatRect& newPosition) :
		skinId("NO SKIN"),
		skin(),
		position(newPosition)
	{
		setSkin(textureManagerId); //Set the entity's skin
	}

	void Entity::updatePosition(const FloatRect& adjustment, const EntityUpdateParam& updateParam) {
		switch (updateParam) {
		case UPDATE_ABSOLUTE:
			position = adjustment;
			break;
		case UPDATE_RELATIVE:
			position.x += adjustment.x;
			position.y += adjustment.y;
			position.w += adjustment.w;
			position.h += adjustment.h;
			position.r += adjustment.r;
			break;
		}
	}

	void Entity::setPosition(int newX, int newY, int newW, int newH, int newR) {
		if (newX != VOID) {
			position.x = newX;
		}
		if (newY != VOID) {
			position.y = newY;
		}
		if (newW != VOID) {
			position.w = newW;
		}
		if (newH != VOID) {
			position.h = newH;
		}
		if (newR != VOID) {
			position.r = newR;
		}
	}

	FloatRect Entity::getPosition() {
		return position;
	}

	bool Entity::checkCollision(const FloatRect& rect) {
		// TODO: Use floats, add rotation support
		SDL_Rect position1 = {position.x, position.y, position.w, position.h};
		SDL_Rect position2 = {rect.x, rect.y, rect.w, rect.h};
		return SDL_HasIntersection(&position1, &position2);
	}

	bool Entity::checkCollision(const Entity& entity) {
		return checkCollision(entity.position);
	}

	bool Entity::onScreen() {
		// TODO: Create more accurate bounding box from rotated position
		Window& window = Window::getInstance();
		SDL_Rect screen = { 0, 0, window.getWidth(), window.getHeight()};
		SDL_Rect position = { this->position.x, this->position.y, this->position.w, this->position.h };
		return SDL_HasIntersection(&position, &screen);
	}

	void Entity::setSkin(const std::string& id) {
		if (textureManager.hasId(id)) { //Check if texture exists
			skinId = id;
			skin = textureManager.getTexture(skinId);
		}
		else {
			throw invalid_id("Texture with id \"" + id + "\" does not exist.");
		}
	}

	std::string Entity::getSkinId() {
		return skinId;
	}

}
