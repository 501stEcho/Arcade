/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** Snake.hpp
*/

#pragma once

#include "../../interface/types/Render.hpp"
#include "../../interface/IGame.hpp"
#include "../GameUtils.hpp"
#include <chrono>
#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace game
{
    #define TIME_TO_MOVE std::chrono::duration<double, std::ratio<1, 10>>(1)
    #define TIME_DELAY std::chrono::duration<double, std::ratio<1, 2>>(1)
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
        {SNAKE, "./assets/nibbler/snake.png", "./assets/nibbler/snake.txt", {68, 68}},
        {GROUND, "./assets/nibbler/ground.png", "./assets/nibbler/ground.txt", {64, 64}}
    };

    const std::string MAP_PATH = "./assets/nibbler/levels/";

    const std::map<char, std::size_t>MAP_OBJECTS = {
        {'#', WALL},
        {'.', FLOOR},
    };

    const std::map<char, std::size_t>MAP_ITEMS = {
        {'o', FRUIT},
    };

    struct SnakeBody {
        Coords pos;
        Costume costume;
    };

    struct Items {
        Coords pos;
        Costume costume;
    };

    struct SpriteElem {
        Coords pos;
        arcade::SpriteCostume costume;
    };

    enum Direction { UP, DOWN, LEFT, RIGHT };

    using Position = Direction;

    class NibblerGame : public IGame
    {
        public:
            NibblerGame();
            ~NibblerGame();
            const std::vector<arcade::Spritesheet> &getSpritesheets() const override;
            void compute(const std::chrono::duration<double>& dtime, arcade::IContext &context) override;
            void handleInput(const arcade::KeyInput &key) override;
            void render(arcade::IContext &context) override;

        private:
            void _displaySnake(arcade::IContext &context);
            void _displayItems(arcade::IContext &context);
            void _displayInfos(arcade::IContext &context);
            void _moveSnake();
            int _determineDirection();
            bool _noDirectObstacle(Costume cost, Direction sens);
            bool _itemsOnSpot(Costume costume, Coords coords);
            bool _snakePartOnSpot(Coords coords);
            void _resetSnake();
            int _autoTurn();
            bool _canMove();
            bool _levelComplete();
            void _restartGame();
            void _nextLevel();
            void _appendTail();
            void _handleLiveTime();
            bool _backgroundOnSpot(arcade::SpriteCostume model, Coords coords);
            void _removeItem(Costume costume, Coords coords);
            Costume _determineTail(SnakeBody &tail, SnakeBody &body);
            Costume _determineNeck(Direction nextDir);
            std::size_t _score = 0;
            std::size_t _level = 0;
            std::size_t _levelTime = 0;
            std::chrono::duration<double> _moveClock = std::chrono::duration<double>::zero();
            std::chrono::duration<double> _levelClock = std::chrono::duration<double>::zero();
            std::vector<SnakeBody> _snake;
            std::vector<SpriteElem> _background;
            std::vector<Items> _items;
            int _lives = 1;
            bool _alive = true;
            bool _requestedDirection = false;
            Direction _directionRequest = RIGHT;
            Direction _direction = RIGHT;
    };
}