/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** IRenderer.hpp
*/

#pragma once

#include "types/Events.hpp"
#include "types/Render.hpp"
#include <vector>

namespace render
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;
        virtual void loadSpritesheets(const std::vector<arcade::Spritesheet> &) = 0;
        virtual std::vector<arcade::Event> pollEvents() = 0;
        virtual void clearWin() = 0;
        virtual void refreshWin() = 0;
        virtual void drawSprite(const arcade::SpriteCostume &, double, double) = 0;
        virtual void drawText(const arcade::TextDisplay &, double, double) = 0;
    };
}