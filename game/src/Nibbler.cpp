/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** Snake.hpp
*/

#include "Nibbler.hpp"
#include <utility>

static void parseMap(
double x, double y, char c, std::vector<game::SpriteElem> &sprites)
{
    if (game::MAP_OBJECTS.find(c) == game::MAP_OBJECTS.end())
        return;
    std::size_t costumeId = game::MAP_OBJECTS.at(c);
    arcade::SpriteCostume costume = game::COSTUMES.at(costumeId);
    sprites.push_back({{x, y + 3}, costume});
}

static void parseFruit(
double x, double y, char c, std::vector<game::Items> &items)
{
    if (game::MAP_ITEMS.find(c) == game::MAP_ITEMS.end())
        return;
    game::Costume costume = static_cast<game::Costume>(game::MAP_ITEMS.at(c));
    items.push_back({{x, y + 3}, costume});
}

game::NibblerGame::NibblerGame()
{
    _score = 0;
    _level = 0;
    _lives = 0;
    _levelTime = 0;
    _alive = true;
    _nextLevel();
}

void game::NibblerGame::_restartGame()
{
    _score = 0;
    _level = 0;
    _lives = 0;
    _levelTime = 0;
    _alive = true;
    _nextLevel();
}

const std::vector<arcade::Spritesheet> &
game::NibblerGame::getSpritesheets() const
{
    return SPRITESHEETS;
}

void game::NibblerGame::compute(
const std::chrono::duration<double> &deltaTime, arcade::IContext &context)
{
    _moveClock += deltaTime;
    _levelClock += deltaTime;
    if ((_lives < 0 || _levelTime <= 0) && _alive) {
        context.saveScore(_level);
        _alive = false;
    }
    if (_alive) {
        _handleLiveTime();
        while (_canMove()) {
            _moveSnake();
        }
    }
}

void game::NibblerGame::handleInput(const arcade::KeyInput &key)
{
    if (key.mapping == arcade::RESTART)
        return _restartGame();
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

void game::NibblerGame::render(arcade::IContext &context)
{
    for (auto &elem : _background) {
        context.drawSprite(elem.costume, elem.pos.x, elem.pos.y);
    }
    _displayInfos(context);
    _displayItems(context);
    _displaySnake(context);
    if (!_alive) {
        context.drawText({"Game Over", 7, 0}, 5, 10);
        if (_lives < 0)
            context.drawText({"No mores lives", 7, 0}, 4, 11);
        if (_levelTime <= 0)
            context.drawText({"Time expired", 7, 0}, 4, 11);
        context.drawText({"Press <RESTART> to restart", 7, 0}, 3, 12);
        return;
    }
}

void game::NibblerGame::_nextLevel()
{
    std::string path;

    _level++;
    _score += ((_levelTime / 40) * 1000);
    _levelTime = 990;
    _moveClock.zero();
    _levelClock.zero();
    path = MAP_PATH + "/level_" + std::to_string(_level);

    _background.clear();
    _items.clear();
    _resetSnake();
    _background = loadMap<SpriteElem>(path + ".txt", parseMap);
    _items = loadMap<game::Items>(path + "_items.txt", parseFruit);
}

game::NibblerGame::~NibblerGame()
{
}

game::Costume game::NibblerGame::_determineTail(
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

game::Costume game::NibblerGame::_determineNeck(Direction nextDir)
{
    if (nextDir != _direction) {
        if (_direction == UP)
            return nextDir == LEFT ? SNAKE_LB : SNAKE_BR;
        else if (_direction == DOWN)
            return nextDir == LEFT ? SNAKE_LT : SNAKE_TR;
        else if (_direction == LEFT)
            return nextDir == UP ? SNAKE_TR : SNAKE_BR;
        else if (_direction == RIGHT)
            return nextDir == UP ? SNAKE_LT : SNAKE_LB;
    } else if (_direction == UP || _direction == DOWN)
        return SNAKE_TB;
    return SNAKE_LR;
}

bool game::NibblerGame::_noDirectObstacle(Costume costume, Direction sens)
{
    Coords coord = _snake[_snake.size() - 1].pos;
    if (sens == LEFT)
        coord.x -= 1;
    if (sens == RIGHT)
        coord.x += 1;
    if (sens == UP)
        coord.y -= 1;
    if (sens == DOWN)
        coord.y += 1;
    return (!_backgroundOnSpot(COSTUMES.at(costume), coord));
}

bool game::NibblerGame::_itemsOnSpot(Costume costume, Coords coords)
{
    for (std::size_t i = 0; i < _items.size(); i++) {
        if (_items[i].costume == costume && coords == _items[i].pos)
            return (true);
    }
    return (false);
}

void game::NibblerGame::_removeItem(Costume costume, Coords coords)
{
    for (std::size_t i = 0; i < _items.size(); i++) {
        if (_items[i].costume == costume && coords == _items[i].pos) {
            _items.erase(_items.begin() + i);
        }
    }
}

int game::NibblerGame::_determineDirection()
{
    if (_requestedDirection && _noDirectObstacle(WALL, _directionRequest)) {
        _snake[_snake.size() - 1].costume = _determineNeck(_directionRequest);
        _direction = _directionRequest;
        _requestedDirection = false;
    } else {
        if (_noDirectObstacle(WALL, _direction)) {
            _snake[_snake.size() - 1].costume = _determineNeck(_direction);
        } else if (_autoTurn()) {
            _requestedDirection = false;
            _snake[_snake.size() - 1].costume =
            _determineNeck(_directionRequest);
            _direction = _directionRequest;
        } else
            return (0);
    }
    return (1);
}

bool game::NibblerGame::_levelComplete()
{
    for (std::size_t i = 0; i < _items.size(); i++) {
        if (_items[i].costume == FRUIT)
            return (false);
    }
    return (true);
}

void game::NibblerGame::_moveSnake()
{
    SnakeBody head = {_snake[_snake.size() - 1].pos, SNAKE_HEAD_T};

    if (!_determineDirection())
        return;
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
    if (_itemsOnSpot(FRUIT, head.pos)) {
        _removeItem(FRUIT, head.pos);
        _appendTail();
    }
    if (_snakePartOnSpot(head.pos)) {
        _lives--;
        if (_lives >= 0) {
            _resetSnake();
            return;
        }
    }
    _snake.erase(_snake.begin());
    _snake[0].costume = _determineTail(_snake[0], _snake[1]);
    _snake.push_back(head);
    if (_levelComplete() && _lives >= 0) {
        _lives++;
        _nextLevel();
    }
}

bool game::NibblerGame::_snakePartOnSpot(Coords coords)
{
    for (auto &part : _snake) {
        if (part.pos == coords)
            return (true);
    }
    return (false);
}

bool game::NibblerGame::_backgroundOnSpot(
arcade::SpriteCostume model, Coords coords)
{
    for (auto &i : _background) {
        if (i.costume == model && i.pos == coords)
            return (true);
    }
    return (false);
}

void game::NibblerGame::_resetSnake()
{
    _snake.clear();
    _requestedDirection = false;
    _directionRequest = RIGHT;
    _direction = RIGHT;
    _snake.push_back({{5, 20}, SNAKE_TAIL_L});
    _snake.push_back({{6, 20}, SNAKE_LR});
    _snake.push_back({{7, 20}, SNAKE_LR});
    _snake.push_back({{8, 20}, SNAKE_HEAD_R});
}

void game::NibblerGame::_appendTail()
{
    typedef struct {
        int x;
        int y;
        Costume ol;
        Costume ne;
    } NewTail;

    SnakeBody &otail = *(_snake.begin());
    SnakeBody ntail;
    Coords opos = _snake.begin()->pos;
    Coords npos;
    std::vector<NewTail> l = {{0, 1, SNAKE_TB, SNAKE_TAIL_B}};

    l.push_back({1, 0, SNAKE_TR, SNAKE_TAIL_R});
    l.push_back({0, -1, SNAKE_TB, SNAKE_TAIL_T});
    l.push_back({-1, 0, SNAKE_TR, SNAKE_TAIL_L});
    for (std::size_t i = 0; i < l.size(); i++) {
        npos = {opos.x + l[i].x, opos.y + l[i].y};
        if (!(_backgroundOnSpot(COSTUMES.at(WALL), npos) ||
            _itemsOnSpot(FRUIT, npos) || _snakePartOnSpot(npos))) {
            otail.costume = l[i].ol;
            ntail.costume = l[i].ne;
            ntail.pos = npos;
            _snake.insert(_snake.begin(), ntail);
            return;
        }
    }
}

void game::NibblerGame::_displaySnake(arcade::IContext &context)
{
    for (auto &part : _snake)
        context.drawSprite(COSTUMES.at(part.costume), part.pos.x, part.pos.y);
}

void game::NibblerGame::_displayItems(arcade::IContext &context)
{
    for (auto &items : _items) {
        context.drawSprite(
        COSTUMES.at(items.costume), items.pos.x, items.pos.y);
    }
}

void game::NibblerGame::_displayInfos(arcade::IContext &context)
{
    context.drawText({"PLAYER1", 7, 0}, 0, 0);
    context.drawText({std::to_string(_score), 7, 0}, 5, 0);
    context.drawText({"LEFT   " + std::to_string(_lives), 7, 0}, 8, 0);
    context.drawText({"TIME :", 7, 0}, 9, 2);
    context.drawText({std::to_string(_levelTime), 7, 0}, 12, 2);
    context.drawText({"WAVE " + std::to_string(_level), 7, 0}, 3, 23);
}

void game::NibblerGame::_handleLiveTime()
{
    if (_levelClock >= std::chrono::duration<double>(TIME_DELAY)) {
        _levelClock -= std::chrono::duration<double>(TIME_DELAY);
        _levelTime -= 10;
    }
}

bool game::NibblerGame::_canMove(void)
{
    if (_moveClock >=
    (TIME_TO_MOVE - std::chrono::duration<double>(0.015 * (1)))) {
        _moveClock -=
        TIME_TO_MOVE - std::chrono::duration<double>(0.015 * (1));
        return true;
    }
    return false;
}

int game::NibblerGame::_autoTurn()
{
    if (_direction == UP || _direction == DOWN) {
        if (_noDirectObstacle(WALL, LEFT) && !_noDirectObstacle(WALL, RIGHT)) {
            _directionRequest = LEFT;
            return (1);
        }
        if (!_noDirectObstacle(WALL, LEFT) && _noDirectObstacle(WALL, RIGHT)) {
            _directionRequest = RIGHT;
            return (1);
        }
    } else if (_direction == LEFT || _direction == RIGHT) {
        if (_noDirectObstacle(WALL, UP) && !_noDirectObstacle(WALL, DOWN)) {
            _directionRequest = UP;
            return (1);
        }
        if (!_noDirectObstacle(WALL, UP) && _noDirectObstacle(WALL, DOWN)) {
            _directionRequest = DOWN;
            return (1);
        }
    }
    return (0);
}

extern "C" {
game::IGame *entryPoint()
{
    return new game::NibblerGame;
}
int getType()
{
    return arcade::GAME;
}
std::string getName()
{
    return "Nibbler";
}
}
