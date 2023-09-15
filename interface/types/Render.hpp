/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** Events.hpp
*/

#pragma once

#include "Global.hpp"
#include <cstddef>
#include <string>

namespace arcade
{
    struct Spritesheet {
        std::size_t id;
        std::string graphicPath;
        std::string textPath;
        Position unitDims;
    };

    struct SpriteCostume {
        std::size_t spritesheet;
        Position pos;
    };

    struct TextDisplay {
        std::string content;
        char fg;
        char bg;
    };
}

inline bool operator==(const arcade::SpriteCostume &model, const arcade::SpriteCostume &costume)
{
    return (model.spritesheet == costume.spritesheet && model.pos == costume.pos);
}