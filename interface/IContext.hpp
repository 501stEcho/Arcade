/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** IContext.hpp
*/

#pragma once

#include "IRenderer.hpp"
#include "types/Events.hpp"
#include "types/Render.hpp"
#include <memory>

namespace arcade
{
    class IContext
    {
    public:
        virtual ~IContext() = default;
        virtual void clearWin() = 0;
        virtual void refreshWin() = 0;
        virtual void drawSprite(const SpriteCostume &sprite, double x, double y) = 0;
        virtual void drawText(const TextDisplay &text, double x, double y) = 0;
        virtual void saveScore(const std::size_t &score) = 0;
    };
}