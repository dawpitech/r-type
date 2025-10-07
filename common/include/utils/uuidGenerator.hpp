//
// EPITECH PROJECT, 2025
// R-type
// File description:
// uuid generator
//

#pragma once

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>

namespace utils
{
    class UuidGenerator
    {
        public:
            static std::string generateUuid()
            {
                boost::uuids::random_generator& gen = generator();
                return boost::uuids::to_string(gen());
            }

        private:
            static boost::uuids::random_generator& generator()
            {
                static boost::uuids::random_generator gen;
                return gen;
            }
    };
} // namespace utils
