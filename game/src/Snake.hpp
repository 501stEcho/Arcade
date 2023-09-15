/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** Snake.hpp
*/

#pragma once

#include "../../interface/types/Render.hpp"
#include "../../interface/IGame.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <iostream>

namespace game
{
    #define TIME_TO_MOVE std::chrono::duration<double, std::ratio<1, 5>>(1)
    enum Spritesheets { SNAKE, GROUND };

    enum Costume {
        SNAKE_BR,
        SNAKE_LR,
        SNAKE_LB,
        SNAKE_HEAD_T,
        SNAKE_HEAD_R,
        SNAKE_TR,
        SNAKE_TB,
        SNAKE_HEAD_L,
        SNAKE_HEAD_B,
        SNAKE_LT,
        SNAKE_TAIL_B,
        SNAKE_TAIL_L,
        FRUIT,
        SNAKE_TAIL_R,
        SNAKE_TAIL_T,
        WALL,
        FLOOR,
    };

    const std::map<std::size_t, arcade::SpriteCostume> COSTUMES = {
    {SNAKE_BR, {SNAKE, {0, 0}}},
    {SNAKE_LR, {SNAKE, {1, 0}}},
    {SNAKE_LB, {SNAKE, {2, 0}}},
    {SNAKE_HEAD_T, {SNAKE, {3, 0}}},
    {SNAKE_HEAD_R, {SNAKE, {4, 0}}},
    {SNAKE_TR, {SNAKE, {0, 1}}},
    {SNAKE_TB, {SNAKE, {2, 1}}},
    {SNAKE_HEAD_L, {SNAKE, {3, 1}}},
    {SNAKE_HEAD_B, {SNAKE, {4, 1}}},
    {SNAKE_LT, {SNAKE, {2, 2}}},
    {SNAKE_TAIL_B, {SNAKE, {3, 2}}},
    {SNAKE_TAIL_L, {SNAKE, {4, 2}}},
    {FRUIT, {SNAKE, {0, 3}}},
    {SNAKE_TAIL_R, {SNAKE, {3, 3}}},
    {SNAKE_TAIL_T, {SNAKE, {4, 3}}},
    {FLOOR, {GROUND, {0, 0}}},
    {WALL, {GROUND, {1, 0}}},
    };

    const std::vector<arcade::Spritesheet> SPRITESHEETS = {
    {SNAKE, "./assets/snake/snake.png", "./assets/snake/snake.txt", {64, 64}},
    {GROUND, "./assets/snake/ground.png", "./assets/snake/ground.txt", {64, 64}}};

    const std::string MAP_PATH = "./assets/snake/map.txt";

    const std::map<char, std::size_t> MAP_OBJECTS = {
    {'#', WALL},
    {'.', FLOOR},
    };

    struct SnakeBody {
        Coords pos;
        Costume costume;
    };

    struct SpriteElem {
        Coords pos;
        arcade::SpriteCostume costume;
    };

    enum Direction { UP, DOWN, LEFT, RIGHT };

    using Position = Direction;

    class SnakeGame : public IGame
    {
    public:
        SnakeGame();
        ~SnakeGame();
        const std::vector<arcade::Spritesheet> &getSpritesheets() const override;
        void compute(const std::chrono::duration<double>& dtime, arcade::IContext &context) override;
        void handleInput(const arcade::KeyInput &key) override;
        void render(arcade::IContext &context) override;

    private:
        void _displayBackground(arcade::IContext &context);
        void _displaySnake(arcade::IContext &context);
        void _moveSnake(arcade::IContext &context);
        void _handleDirection();
        Costume _determineTail(SnakeBody &tail, SnakeBody &body);
        Costume _determineNeck();
        void _moveFruit();
        void _enlargeSnake();
        bool _isFruitEaten();
        bool _fruitOnSnake();
        bool _isSnakeDead();
        bool _shouldMove();
        void _restartGame();
        std::vector<SnakeBody> _snake;
        const std::vector<SpriteElem> _background;
        bool _requestedDirection = false;
        Direction _directionRequest = RIGHT;
        Direction _direction = RIGHT;
        Coords _fruitPos = {0,0};
        bool _hasEaten = false;
        bool _isDead = false;
        std::chrono::duration<double> _timeReminder = std::chrono::duration<double>::zero();
    };
}