/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TextureStore.hpp
*/

#pragma once

#include <raylib-cpp.hpp>

#include <unordered_map>
#include <functional>
#include <tuple>

#include <memory>

class TextureStore {
    public:
        struct SubTextureKeyHash {
            std::size_t operator()(const std::tuple<std::string, int, int, int, int>& k) const {
                // Combine hashes for all tuple fields
                size_t h1 = std::hash<std::string>{}(std::get<0>(k));
                size_t h2 = std::hash<int>{}(std::get<1>(k));
                size_t h3 = std::hash<int>{}(std::get<2>(k));
                size_t h4 = std::hash<int>{}(std::get<3>(k));
                size_t h5 = std::hash<int>{}(std::get<4>(k));
                return (((((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1)) >> 1) ^ (h4 << 1)) ^ (h5 << 1);
            }
        };

        static TextureStore& getInstance() {
            static TextureStore instance;
            return instance;
        }

        TextureStore(const TextureStore&) = delete;
        TextureStore& operator=(const TextureStore&) = delete;

        raylib::Texture& getTexture(const std::string& assetPath, int startX, int startY, int width, int height) {
            auto tuple = std::make_tuple(assetPath, startX, startY, width, height);
            if (this->_store.contains(tuple))
                return this->_store.at(tuple);

            raylib::Image image(assetPath);

            width = (width <= 0) ? image.width : width;
            height = (height <= 0) ? image.height : height;

            image.Crop({
                static_cast<float>(startX),
                static_cast<float>(startY),
                static_cast<float>(width),
                static_cast<float>(height),
            });

            raylib::Texture cropped(image);

            this->_store.insert({tuple, std::move(cropped)});
            return this->_store.at(tuple);
        }

    private:
        std::unordered_map<std::tuple<std::string, int, int, int, int>, raylib::Texture, SubTextureKeyHash> _store;

        TextureStore() : _store() {}
        ~TextureStore() = default;
};
