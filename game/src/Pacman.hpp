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
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

bool is_integer(double n);
bool samePos(Coords pos1, Coords pos2);

namespace game
{
    #define TIME_TO_MOVE std::chrono::duration<double, std::ratio<1, 50>>(2)
    #define TIME_DELAY std::chrono::duration<double, std::ratio<1, 2>>(1)
    enum Spritesheets { MAIN, GROUND };

    enum Costume {
        FRUIT_1,
        FRUIT_2,
        PACMAN_UP,
        PACMAN_DOWN,
        PACMAN_RIGHT,
        PACMAN_LEFT,
        GHOST_1,
        GHOST_1_FEAR,
        GHOST_2,
        GHOST_2_FEAR,
        GHOST_3,
        GHOST_3_FEAR,
        GHOST_4,
        GHOST_4_FEAR,
        WALL,
        FLOOR,
    };

    const std::map<std::size_t, arcade::SpriteCostume> COSTUMES = {
        {FRUIT_1, {MAIN, {0, 3}}},
        {FRUIT_2, {MAIN, {0, 3}}},
        {PACMAN_UP, {MAIN, {3, 0}}},
        {PACMAN_RIGHT, {MAIN, {4, 0}}},
        {PACMAN_LEFT, {MAIN, {3, 1}}},
        {PACMAN_DOWN,{MAIN, {4, 1}}},
        {GHOST_1, {MAIN, {0, 0}}},
        {GHOST_1_FEAR, {MAIN, {0, 1}}},
        {GHOST_2, {MAIN, {0, 0}}},
        {GHOST_2_FEAR, {MAIN, {1, 0}}},
        {GHOST_3, {MAIN, {0, 0}}},
        {GHOST_3_FEAR, {MAIN, {2, 1}}},
        {GHOST_4, {MAIN, {0, 0}}},
        {GHOST_4_FEAR, {MAIN, {0, 2}}},
        {WALL, {GROUND, {1, 0}}},
        {FLOOR, {GROUND, {0, 0}}}
    };

    const std::vector<arcade::Spritesheet> SPRITESHEETS = {
        {MAIN, "assets/pacman/main.png", "assets/pacman/main.txt", {68, 68}},
        {GROUND, "assets/pacman/ground.png", "assets/pacman/ground.txt", {64, 64}}
    };

    #define BOARD_WOFFSET 0
    #define BOARD_HOFFSET 3

    #define PACMAN_SPEED 0.1
    #define GHOST_SPEED 0.1

    #define GHOST_NB 4
    
    const std::string MAP_PATH = "assets/pacman/levels/";

    enum EntityType { PACMAN, GHOST, FRUIT, PACGUM };

    enum Direction { UP, DOWN, LEFT, RIGHT };

    enum IAType { RANDOM, PATROL, SEMI, HEATSEAKING };

    const std::map<char, std::size_t> MAP_OBJECTS = {
        {'#', WALL},
        {'.', FLOOR},
    };

    const std::map<char, EntityType> MAP_ITEMS = {
        {'o', FRUIT},
        {'Q', PACGUM}
    };

    struct Entity {
        void move();
        Coords pos;
        Costume costume;
        Direction direction;
        EntityType type;
        double speed;
    };

    struct Ghost : public Entity {
        IAType behaviour;
    };

    struct SpriteElem {
        Coords pos;
        arcade::SpriteCostume costume;
    };

    using Position = Direction;

    class PacmanGame : public IGame
    {
        public:
            PacmanGame();
            ~PacmanGame();
            const std::vector<arcade::Spritesheet> &getSpritesheets() const override;
            void compute(const std::chrono::duration<double>& dtime, arcade::IContext &context) override;
            void handleInput(const arcade::KeyInput &key) override;
            void render(arcade::IContext &context) override;

        private:
            void _displayPacman(arcade::IContext &context);
            void _displayGhosts(arcade::IContext &context);
            void _displayItems(arcade::IContext &context);
            void _displayInfos(arcade::IContext &context);
            void _resetPacman();
            void _resetGhosts();
            void _resetItems();
            int _determineDirection();
            void _determineGhostDirection(std::shared_ptr<Ghost> ghost);
            bool _noDirectObstacle(Direction sens);
            void _moveEntities(void);
            void _movePacman(void);
            void _moveGhosts(void);
            bool _itemsOnSpot(EntityType type, Coords coords);
            bool _ghostOnSpot(Coords coords);
            int _autoTurn();
            void _checkCollision();
            bool _canMove();
            bool _levelComplete();
            void _restartGame();
            void _nextLevel();
            void _initGhosts();
            void _determinePacmanCostume(void);
            void _determineGhostsCostume(void);
            void _determineAllCostume(void);
            bool _backgroundOnSpot(arcade::SpriteCostume model, Coords coords);
            void _removeItem(EntityType costume, Coords coords);
            Direction _getInverseDirect(Direction direct);
            void _getAllButInverse(std::vector<Direction> &direct, Direction actu);
            void _getAvailableDirect(std::vector<Direction> &direct, Coords pos);
            std::size_t _score;
            std::size_t _level;
            std::chrono::duration<double> _moveClock;
            std::shared_ptr<Entity> _pacman;
            std::vector<std::shared_ptr<Ghost>> _ghosts;
            std::vector<SpriteElem> _background;
            std::vector<std::shared_ptr<Entity>> _items;
            int _lives;
            bool _alive;
            bool _requestedDirection = false;
            Direction _directionRequest = RIGHT;
            Direction _direction = RIGHT;
    };
}