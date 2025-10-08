//
// EPITECH PROJECT, 2025
// r-type
// File description:
// spriteHandler
//

#include <SDL3/SDL_render.h>
#include "sdlManager.hpp"

#include "spriteHandler.hpp"

constexpr uint8_t NB_PLAYER_SPRITE_X = 5;
constexpr uint8_t NB_PLAYER_SPRITE_Y = 5;
constexpr uint8_t MOB_1_Y_EMPLACEMENT = 110;
constexpr uint8_t NB_MOB_1_SPRITE_X = 4;
constexpr uint8_t MOB_1_SIZE_Y = 40;

sprite::SpriteHandler::SpriteHandler()
{
    this->_setPlayerSprite(render::SDLManager::getRenderer());
    this->_setMobSprite(render::SDLManager::getRenderer());
}

void sprite::SpriteHandler::_setPlayerSprite(SDL_Renderer* renderer)
{
    this->_playerSprite.texture = render::SDLManager::load("./assets/player.gif");

    this->_playerSprite.size.x = 332;
    this->_playerSprite.size.y = 172;
    const float frameWidth = this->_playerSprite.size.x / NB_PLAYER_SPRITE_X;
    const float frameHeight = this->_playerSprite.size.y / NB_PLAYER_SPRITE_Y;
    this->_playerSprite.frameSize.x = frameWidth;
    this->_playerSprite.frameSize.y = frameHeight;
    for (int i = 0; i < NB_PLAYER_SPRITE_X; i++) {
        Rect rect{static_cast<float>(i) * frameWidth, 0, frameWidth, frameHeight};
        this->_playerSprite.spriteMap.emplace_back(rect);
    }
}

sprite::SpriteData sprite::SpriteHandler::getPlayerSprite() const { return this->_playerSprite; }

void sprite::SpriteHandler::_setMobSprite(SDL_Renderer* renderer)
{
    this->_mobSprite.texture = render::SDLManager::load("./assets/mob1.gif");

    this->_mobSprite.size.x = 200;
    this->_mobSprite.size.y = 154;
    const float frameWidth = this->_mobSprite.size.x / NB_MOB_1_SPRITE_X;
    const float frameHeight = MOB_1_SIZE_Y;
    this->_mobSprite.frameSize.x = frameWidth;
    this->_mobSprite.frameSize.y = frameHeight;
    for (int i = 0; i < NB_MOB_1_SPRITE_X; i++) {
        sprite::Rect rect{static_cast<float>(i) * frameWidth, MOB_1_Y_EMPLACEMENT, frameWidth, frameHeight};
        this->_mobSprite.spriteMap.emplace_back(rect);
    }
}

sprite::SpriteData sprite::SpriteHandler::getMobSprite() const { return this->_mobSprite; }
