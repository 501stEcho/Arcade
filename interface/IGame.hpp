/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** AGame.hpp
*/

#pragma once

#include "types/Events.hpp"
#include "types/Render.hpp"
#include "IContext.hpp"
#include <cstddef>
#include <vector>
#include <chrono>

namespace game
{
    class IGame
    {
    public:
        virtual ~IGame() = default;
        virtual const std::vector<arcade::Spritesheet> &getSpritesheets() const = 0;
        virtual auto compute(const std::chrono::duration<double>& dtime, arcade::IContext &context) -> void = 0;
        virtual void handleInput(const arcade::KeyInput &key) = 0;
        virtual void render(arcade::IContext &context) = 0;
    };
}
