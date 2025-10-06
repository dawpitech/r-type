//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Observer classes
//

#pragma once

namespace utils
{
    template <typename T>
    class IObserver
    {
        public:
            virtual ~IObserver() = default;

            virtual void update(const T &data) = 0;
    };

    template <typename T>
    class ISubject
    {
        public:
            virtual ~ISubject() = default;

            virtual void attach(IObserver<T>& observer) = 0;
            virtual void notify(const T& data) = 0;
    };
} // namespace utils
