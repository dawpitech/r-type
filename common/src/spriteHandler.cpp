//
// EPITECH PROJECT, 2025
// r-type
// File description:
// spriteHandler
//

#include <SDL3/SDL_render.h>
#include <string>
#include "sdlManager.hpp"

#include "spriteHandler.hpp"

constexpr uint8_t NB_PLAYER_SPRITE_X = 5;
constexpr uint8_t NB_PLAYER_SPRITE_Y = 5;
constexpr uint8_t MOB_1_Y_EMPLACEMENT = 110;
constexpr uint8_t NB_MOB_1_SPRITE_X = 4;
constexpr uint8_t MOB_1_SIZE_Y = 40;


void render::SpriteHandler::setSprite(render::SpriteData &sprite, const std::string& path)
{
    if (path == "./assets/player.gif") {
        this->_setPlayerSprite(sprite);
    }
    if (path == "./assets/mob1.gif") {
        this->_setMobSprite(sprite);
    }
}

void render::SpriteHandler::_setPlayerSprite(render::SpriteData &sprite)
{
    sprite.size.x = 332;
    sprite.size.y = 172;
    const float frameWidth = sprite.size.x / NB_PLAYER_SPRITE_X;
    const float frameHeight = sprite.size.y / NB_PLAYER_SPRITE_Y;
    sprite.frameSize.x = frameWidth;
    sprite.frameSize.y = frameHeight;
    for (int i = 0; i < NB_PLAYER_SPRITE_X; i++) {
        Rect rect{static_cast<float>(i) * frameWidth, 0, frameWidth, frameHeight};
        sprite.spriteMap.emplace_back(rect);
    }
}

void render::SpriteHandler::_setMobSprite(render::SpriteData &sprite)
{
    sprite.size.x = 200;
    sprite.size.y = 154;
    const float frameWidth = sprite.size.x / NB_MOB_1_SPRITE_X;
    const float frameHeight = MOB_1_SIZE_Y;
    sprite.frameSize.x = frameWidth;
    sprite.frameSize.y = frameHeight;
    for (int i = 0; i < NB_MOB_1_SPRITE_X; i++) {
        Rect rect{static_cast<float>(i) * frameWidth, MOB_1_Y_EMPLACEMENT, frameWidth, frameHeight};
        sprite.spriteMap.emplace_back(rect);
    }
}
