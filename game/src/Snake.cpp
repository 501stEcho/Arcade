/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** Snake.hpp
*/

#include "Snake.hpp"
#include "../GameUtils.hpp"
#include <cstddef>
#include <curses.h>
#include <utility>
#include <vector>

static void parseMap(
double x, double y, char c, std::vector<game::SpriteElem> &sprites)
{
    if (game::MAP_OBJECTS.find(c) == game::MAP_OBJECTS.end())
        return;
    std::size_t costumeId = game::MAP_OBJECTS.at(c);
    arcade::SpriteCostume costume = game::COSTUMES.at(costumeId);
    sprites.push_back({{x, y + 2}, costume});
}

const std::vector<arcade::Spritesheet> &
game::SnakeGame::getSpritesheets() const
{
    return SPRITESHEETS;
}

game::SnakeGame::SnakeGame()
    : _background(loadMap<SpriteElem>(MAP_PATH, parseMap))
{
    _snake.clear();
    _snake.push_back({{5, 5}, SNAKE_TAIL_L});
    _snake.push_back({{6, 5}, SNAKE_LR});
    _snake.push_back({{7, 5}, SNAKE_LR});
    _snake.push_back({{8, 5}, SNAKE_HEAD_R});
    _requestedDirection = false;
    _direction = RIGHT;
    _directionRequest = RIGHT;
    _moveFruit();
    _isDead = false;
}

game::SnakeGame::~SnakeGame()
{
}

game::Costume game::SnakeGame::_determineTail(
game::SnakeBody &tail, game::SnakeBody &body)
{
    Position pos;
    Costume &costume = body.costume;
    if (tail.pos.x < body.pos.x)
        pos = LEFT;
    else if (tail.pos.x > body.pos.x)
        pos = RIGHT;
    else if (tail.pos.y < body.pos.y)
        pos = UP;
    else if (tail.pos.y > body.pos.y)
        pos = DOWN;

    if (pos == UP &&
    (costume == SNAKE_TB || costume == SNAKE_TR || costume == SNAKE_LT))
        return SNAKE_TAIL_T;
    else if (pos == DOWN &&
    (costume == SNAKE_TB || costume == SNAKE_BR || costume == SNAKE_LB))
        return SNAKE_TAIL_B;
    else if (pos == LEFT &&
    (costume == SNAKE_LR || costume == SNAKE_LB || costume == SNAKE_LT))
        return SNAKE_TAIL_L;
    return SNAKE_TAIL_R;
}

game::Costume game::SnakeGame::_determineNeck()
{
    if (_requestedDirection) {
        if (_direction == UP)
            return _directionRequest == LEFT ? SNAKE_LB : SNAKE_BR;
        else if (_direction == DOWN)
            return _directionRequest == LEFT ? SNAKE_LT : SNAKE_TR;
        else if (_direction == LEFT)
            return _directionRequest == UP ? SNAKE_TR : SNAKE_BR;
        else if (_direction == RIGHT)
            return _directionRequest == UP ? SNAKE_LT : SNAKE_LB;
    } else if (_direction == UP || _direction == DOWN)
        return SNAKE_TB;
    return SNAKE_LR;
}

void game::SnakeGame::_moveSnake(arcade::IContext &context)
{
    SnakeBody head = {_snake[_snake.size() - 1].pos, _snake[_snake.size() - 1].costume};
    if (_isSnakeDead()) {
        _isDead = true;
        context.saveScore(_snake.size() - 4);
        return;
    }
    if (!_hasEaten) {
        _snake.erase(_snake.begin());
        _snake[0].costume = _determineTail(_snake[0], _snake[1]);
    }
    _hasEaten = false;
    _snake[_snake.size() - 1].costume = _determineNeck();
    if (_requestedDirection) {
        _direction = _directionRequest;
        _requestedDirection = false;
    }
    if (_direction == UP) {
        head.pos.y -= 1;
        head.costume = SNAKE_HEAD_T;
    } else if (_direction == DOWN) {
        head.pos.y += 1;
        head.costume = SNAKE_HEAD_B;
    } else if (_direction == LEFT) {
        head.pos.x -= 1;
        head.costume = SNAKE_HEAD_L;
    } else if (_direction == RIGHT) {
        head.pos.x += 1;
        head.costume = SNAKE_HEAD_R;
    }
    _snake.push_back(head);
}

void game::SnakeGame::_displaySnake(arcade::IContext &context)
{
    for (auto &part : _snake)
        context.drawSprite(COSTUMES.at(part.costume), part.pos.x, part.pos.y);
}

void game::SnakeGame::_displayBackground(arcade::IContext &context)
{
    for (auto &elem : _background)
        context.drawSprite(elem.costume, elem.pos.x, elem.pos.y);
}
void game::SnakeGame::handleInput(const arcade::KeyInput &key)
{
    if (_requestedDirection || _isDead)
        return;
    if (_direction != DOWN && _direction != UP) {
        if (key.mapping == arcade::UP) {
            _directionRequest = UP;
            _requestedDirection = true;
        } else if (key.mapping == arcade::DOWN) {
            _directionRequest = DOWN;
            _requestedDirection = true;
        }
    } else if (_direction != RIGHT && _direction != LEFT) {
        if (key.mapping == arcade::LEFT) {
            _directionRequest = LEFT;
            _requestedDirection = true;
        } else if (key.mapping == arcade::RIGHT) {
            _directionRequest = RIGHT;
            _requestedDirection = true;
        }
    }
}

bool game::SnakeGame::_fruitOnSnake()
{
    for (auto &part : _snake)
        if (part.pos == _fruitPos)
            return true;
    return false;
}

void game::SnakeGame::_moveFruit()
{
    Coords lastPos = _fruitPos;
    do {
        _fruitPos.x = 1 + rand() % 25;
        _fruitPos.y = 3 + rand() % 25;
    } while (_fruitPos == lastPos && _fruitOnSnake());
}

bool game::SnakeGame::_isFruitEaten()
{
    return _snake[_snake.size() - 1].pos == _fruitPos;
}

bool game::SnakeGame::_shouldMove()
{
    if (_timeReminder >= TIME_TO_MOVE) {
        _timeReminder -= TIME_TO_MOVE;
        return true;
    }
    return false;
}

void game::SnakeGame::compute(
const std::chrono::duration<double> &deltaTime, arcade::IContext &context)
{
    if (_isDead)
        return;
    _timeReminder += deltaTime;
    while (_shouldMove())
        _moveSnake(context);
    if (_isFruitEaten()) {
        _moveFruit();
        _hasEaten = true;
    }
}

void game::SnakeGame::render(arcade::IContext &context)
{
    _displayBackground(context);
    context.drawSprite(COSTUMES.at(FRUIT), _fruitPos.x, _fruitPos.y);
    context.drawText({"Snake: ", 7, 0}, 0, 0);
    context.drawText({std::to_string(_snake.size() - 4), 7, 0}, 3, 0);
    _displaySnake(context);

    if (_isDead) {
        context.drawText({"Game Over", 7, 0}, 5, 10);
        context.drawText({"Press <RESTART> to restart", 7, 0}, 3, 11);
        return;
    }
    _handleDirection();
}
void game::SnakeGame::_handleDirection()
{
}

bool game::SnakeGame::_isSnakeDead()
{
    Coords head = _snake[_snake.size() - 1].pos;
    Direction direction = _direction;
    if (_requestedDirection)
        direction = _directionRequest;
    if (direction == UP)
        head.y -= 1;
    else if (direction == DOWN)
        head.y += 1;
    else if (direction == LEFT)
        head.x -= 1;
    else if (direction == RIGHT)
        head.x += 1;

    if (head.x < 1 || head.x > 25 || head.y < 3 || head.y > 27)
        return true;
    for (std::size_t i = 0; i < _snake.size() - 1; i++)
        if (_snake[i].pos == head)
            return true;
    return false;
}

extern "C" {
game::IGame *entryPoint()
{
    return new game::SnakeGame;
}
int getType()
{
    return arcade::GAME;
}
std::string getName()
{
    return "Snake";
}
}
