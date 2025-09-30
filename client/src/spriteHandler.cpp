//
// EPITECH PROJECT, 2025
// r-type
// File description:
// spriteHandler
//

#include "client/spriteHandler.hpp"
#include <cstdint>
#include <iostream>
#include <ostream>
#include "SDL3/SDL_render.h"

constexpr uint8_t nbPlayerSpriteX = 6;
constexpr uint8_t nbPlayerSpriteY = 5;

sprite::SpriteHandler::SpriteHandler(utils::TextureManager& textureManager, SDL_Renderer* renderer) :
    _textureManager(textureManager)
{
    this->_setPlayerSprite(renderer);
    this->_setMobSprite(renderer);
}

void sprite::SpriteHandler::_setPlayerSprite(SDL_Renderer* renderer)
{
    this->_playerSprite.texture = this->_textureManager.load(renderer, "./assets/player.gif");

    this->_playerSprite.size.x = 332;
    this->_playerSprite.size.y = 172;
    const int frameWidth = this->_playerSprite.size.x / nbPlayerSpriteX;
    const int frameHeight = this->_playerSprite.size.y / nbPlayerSpriteY;
    sprite::Rect rect{0, 0, 0, 0};
    for (int i = 0; i < nbPlayerSpriteX; i++) {
        sprite::Rect rect{i * frameWidth, 0, frameWidth, frameHeight};
        this->_playerSprite.spriteMap.push_back(rect);
    }
}

sprite::SpriteData sprite::SpriteHandler::getPlayerSprite() const { return this->_playerSprite; }

void sprite::SpriteHandler::_setMobSprite(SDL_Renderer* renderer)
{
    this->_playerSprite.size.x = 332;
    this->_playerSprite.size.y = 172;
    const int frameWidth = this->_playerSprite.size.x / nbPlayerSpriteX;
    const int frameHeight = this->_playerSprite.size.y / nbPlayerSpriteY;
    sprite::Rect rect{0, 0, 0, 0};
    for (int i = 0; i < nbPlayerSpriteX; i++) {
        sprite::Rect rect{i * frameWidth, 0, frameWidth, frameHeight};

        std::cout << "Frame " << i << ": srcX=" << rect.srcX << ", srcY=" << rect.srcY << ", width=" << rect.destX
                  << ", height=" << rect.destY << std::endl;

        this->_playerSprite.spriteMap.push_back(rect);
    }
}

sprite::SpriteData sprite::SpriteHandler::getMobSprite() const { return this->_playerSprite; }
