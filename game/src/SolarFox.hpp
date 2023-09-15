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
#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace game
{
    #define TIME_TO_MOVE std::chrono::duration<double, std::ratio<1, 100>>(2)
    #define TIME_TO_INTERACT std::chrono::duration<double, std::ratio<1, 100>>(7)

    #define DELAY_TO_SHOOT std::chrono::duration<double, std::ratio<1, 1000000>>(1)

    #define BOARD_WIDTH 14
    #define BOARD_HEIGHT 11

    #define BOARD_WOFFSET 3
    #define BOARD_HOFFSET 6

    #define ALLY_SPEED 0.08
    #define FOE_SPEED 0.1

    enum Spritesheets { MAIN };

    enum Costume {
        SHIP_RIGHT_1,
        SHIP_RIGHT_2,
        SHIP_LEFT_1,
        SHIP_LEFT_2,
        COIN_BASE,
        COIN_LV2,
        ENNEMY_UP_1,
        ENNEMY_UP_2,
        ENNEMY_DOWN_1,
        ENNEMY_DOWN_2,
        ENNEMY_RIGHT_1,
        ENNEMY_RIGHT_2,
        ENNEMY_LEFT_1,
        ENNEMY_LEFT_2,
        FOELASER,
        ALLYLASER,
        SKY,
        MINE
    };

    const std::map<std::size_t, arcade::SpriteCostume> COSTUMES = {
        {SHIP_RIGHT_1, {MAIN, {0, 0}}},
        {SHIP_RIGHT_2, {MAIN, {1, 0}}},
        {SHIP_LEFT_1, {MAIN, {2, 0}}},
        {SHIP_LEFT_2, {MAIN, {3, 0}}},
        {COIN_BASE, {MAIN, {4, 0}}},
        {COIN_LV2, {MAIN, {0, 1}}},
        {ENNEMY_UP_1, {MAIN, {1, 1}}},
        {ENNEMY_UP_2, {MAIN, {2, 1}}},
        {ENNEMY_DOWN_1, {MAIN, {3, 1}}},
        {ENNEMY_DOWN_2, {MAIN, {4, 1}}},
        {ENNEMY_RIGHT_1, {MAIN, {0, 2}}},
        {ENNEMY_RIGHT_2, {MAIN, {1, 2}}},
        {ENNEMY_LEFT_1, {MAIN, {2, 2}}},
        {ENNEMY_LEFT_2, {MAIN, {3, 2}}},
        {FOELASER, {MAIN, {4, 2}}},
        {ALLYLASER, {MAIN, {0, 3}}},
        {SKY, {MAIN, {1, 3}}},
        {MINE, {MAIN, {2, 3}}},
    };

    const std::vector<arcade::Spritesheet> SPRITESHEETS = {
        {MAIN, "./assets/solarfox/spritesheet.png", "./assets/solarfox/spritesheet.txt", {68, 68}},
    };

    enum Faction { ALLY, FOE };

    enum EntityType { SHIP, BULLET, ITEM };

    enum ItemType { COIN, SPECICOIN, BOMB };

    enum Direction { UP, DOWN, LEFT, RIGHT };

    enum ShipState { ALIVE, STUNNED, DEAD };

    enum ItemState { INTACT, CONSUMED };
    
    enum GameState { WIN, LOSS, UNKNOWN };

    const std::string MAP_PATH = "./assets/nibbler/levels/";

    const std::map<char, ItemType>MAP_ITEMS = {
        {'1', COIN},
        {'2', SPECICOIN},
        {'x', BOMB}
    };


    bool samePos(Coords pos1, Coords pos2);

    struct Entity {
        virtual void move(Direction direction) = 0;
        virtual bool display() = 0;
        virtual bool exists() = 0;
        virtual GameState winLose() = 0;
        virtual std::string getMessage() = 0;
        virtual void setCostume() = 0;
        virtual void interact(std::vector<std::shared_ptr<Entity>> &env) = 0;
        virtual void handleEnv(std::vector<std::shared_ptr<Entity>> &env) = 0;
        virtual void handleInput(const arcade::KeyInput &key, std::vector<std::shared_ptr<Entity>> &env) = 0;
        Coords pos;
        Costume costume;
        Direction direction;
        EntityType type;
        double speed;
    };

    struct Ship;
    struct Bullet;
    struct Item;

    struct Ship : public Entity {
        void move(Direction direction) override;
        bool display() override;
        bool exists() override;
        GameState winLose() override;
        std::string getMessage() override;
        void setCostume() override;
        void interact(std::vector<std::shared_ptr<Entity>> &env) override;
        void handleEnv(std::vector<std::shared_ptr<Entity>> &env) override;
        void handleInput(const arcade::KeyInput &key, std::vector<std::shared_ptr<Entity>> &env);
        void handleCollision(std::shared_ptr<Ship> other);
        void handleCollision(std::shared_ptr<Bullet> other);
        void handleCollision(std::shared_ptr<Item> other);
        void maybeShoot(std::vector<std::shared_ptr<Entity>> &env);
        void shoot(std::vector<std::shared_ptr<Entity>> &env);
        std::string deathmsg;
        Faction faction;
        ShipState state;
    };

    struct Bullet : public Entity {
        void move(Direction direction) override;
        bool display() override;
        bool exists() override;
        GameState winLose() override;
        std::string getMessage() override;
        void setCostume() override;
        void interact(std::vector<std::shared_ptr<Entity>> &env) override;
        void handleEnv(std::vector<std::shared_ptr<Entity>> &env) override;
        void handleInput(const arcade::KeyInput &key, std::vector<std::shared_ptr<Entity>> &env);
        void handleCollision(std::shared_ptr<Ship> other);
        void handleCollision(std::shared_ptr<Bullet> other);
        void handleCollision(std::shared_ptr<Item> other);
        Faction faction;
        double range;
    };

    struct Item : public Entity {
        void move(Direction direction) override;
        bool display() override;
        bool exists() override;
        GameState winLose() override;
        std::string getMessage() override;
        void setCostume() override;
        void interact(std::vector<std::shared_ptr<Entity>> &env) override;
        void handleEnv(std::vector<std::shared_ptr<Entity>> &env) override;
        void handleInput(const arcade::KeyInput &key, std::vector<std::shared_ptr<Entity>> &env);
        void handleCollision(std::shared_ptr<Ship> other);
        void handleCollision(std::shared_ptr<Bullet> other);
        void handleCollision(std::shared_ptr<Item> other);
        ItemType itemtype;
        ItemState state;
    };

    struct SpriteElem {
        Coords pos;
        arcade::SpriteCostume costume;
    };

    void _applySpeed(Entity &mob);
    std::shared_ptr<Entity> initItem(ItemType, Coords);

    using Position = Direction;

    class SolarFoxGame : public IGame
    {
        public:
            SolarFoxGame();
            ~SolarFoxGame();
            const std::vector<arcade::Spritesheet> &getSpritesheets() const override;
            void compute(const std::chrono::duration<double>& dtime, arcade::IContext &context) override;
            void handleInput(const arcade::KeyInput &key) override;
            void render(arcade::IContext &context) override;

        private:
            void _restartGame();
            void _nextLevel();
            bool _levelComplete();
            void _moveItems();
            void _interactItems();
            void _cleanItems();
            void _resetBoard();
            void _resetShips();
            void _resetCoins();
            std::shared_ptr<Entity> _createAlly(void);
            std::shared_ptr<Entity> _createFoe(std::size_t id);
            void _handleDirection(void);
            void _displayItems(arcade::IContext &context);
            void _displayInfos(arcade::IContext &context);
            bool _canMove(void);
            bool _canInteract(void);
            void _checkGameState();
            std::size_t _score;
            std::size_t _level;
            std::chrono::duration<double> _moveClock = std::chrono::duration<double>::zero();
            std::chrono::duration<double> _interactClock = std::chrono::duration<double>::zero();
            std::vector<SpriteElem> _background;
            std::vector<std::shared_ptr<Entity>> _items;
            int _lives;
            bool _alive;
            bool _requestedDirection = false;
            Direction _directionRequest = RIGHT;
            Direction _direction = RIGHT;
            bool _saved = false;
    };
}