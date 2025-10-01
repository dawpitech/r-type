//
// EPITECH PROJECT, 2025
// r-type
// File description:
// spriteHandler
//

#include "client/spriteHandler.hpp"
#include <cstdint>
#include "SDL3/SDL_render.h"

constexpr uint8_t nbPlayerSpriteX = 5;
constexpr uint8_t nbPlayerSpriteY = 5;

sprite::SpriteHandler::SpriteHandler(utils::TextureManager& textureManager, SDL_Renderer* renderer) :
    _textureManager(textureManager)
{
    this->_setPlayerSprite(renderer);
}

void sprite::SpriteHandler::_setPlayerSprite(SDL_Renderer* renderer)
{
    this->_playerSprite.texture = this->_textureManager.load(renderer, "./assets/player.gif");

    this->_playerSprite.size.x = 332;
    this->_playerSprite.size.y = 172;
    const float frameWidth = this->_playerSprite.size.x / nbPlayerSpriteX;
    const float frameHeight = this->_playerSprite.size.y / nbPlayerSpriteY;
    for (int i = 0; i < nbPlayerSpriteX; i++) {
        sprite::Rect rect{static_cast<float>(i) * frameWidth, 0, frameWidth, frameHeight};
        this->_playerSprite.spriteMap.push_back(rect);
    }
}

sprite::SpriteData sprite::SpriteHandler::getPlayerSprite() const { return this->_playerSprite; }

void sprite::SpriteHandler::_setMobSprite(SDL_Renderer* renderer) {}

sprite::SpriteData sprite::SpriteHandler::getMobSprite() const { return this->_playerSprite; }
