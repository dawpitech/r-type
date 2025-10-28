//
// EPITECH PROJECT, 2025
// r-type
// File description:
// MusicSystem
//

#include <raylib-cpp.hpp>

#include "components/music.hpp"
#include "flux/core/flux.hpp"

flux::View MusicSystemView(const flux::ECS& ecs) { return ecs.GenerateViewFromComponents<component::MusicCmp>(); }

void MusicSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto music = ecs.GetComponent<component::MusicCmp>(entity);
        if (!music.isPlaying) {
            SetMasterVolume(1.0f);
            PlayMusicStream(music.music);
            music.isPlaying = true;
            ecs.AddOrReplace(entity, music);
        }
        std::cout << "Update Music\n";
        UpdateMusicStream(music.music);
        std::cout << "MUSIC Updated\n";
    }
}
