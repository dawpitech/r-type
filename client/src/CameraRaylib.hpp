/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** CameraStore.hpp
*/

#pragma once

#include <raylib-cpp.hpp>

class CameraRaylib {
    public:

        static CameraRaylib& getInstance() {
            static CameraRaylib instance;
            return instance;
        }

        static raylib::Camera2D& getCamera() { return getInstance()._camera; }

        CameraRaylib(const CameraRaylib&) = delete;
        CameraRaylib& operator=(const CameraRaylib&) = delete;

    private:
        raylib::Camera2D _camera;

        CameraRaylib() : _camera() {}
        ~CameraRaylib() {};
};
