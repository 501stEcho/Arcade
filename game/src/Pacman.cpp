/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** Snake.hpp
*/

#include "Pacman.hpp"
#include <utility>

bool is_integer(double n)
{
    double margin = 0.1;
    double r = fmod(n, 1.0);
    return (r < margin);
}

bool samePos(Coords pos1, Coords pos2)
{
    double margin = 0.5;

    if (std::abs(pos1.x - pos2.x) < margin) {
        if (std::abs(pos1.y - pos2.y) < margin) {
            return (true);
        }
    }
    return (false);
}

double displayround(double n)
{
    return (n - fmod(n, 0.1));
}

static void parseMap(
double x, double y, char c, std::vector<game::SpriteElem> &sprites)
{
    if (game::MAP_OBJECTS.find(c) == game::MAP_OBJECTS.end())
        return;
    std::size_t costumeId = game::MAP_OBJECTS.at(c);
    arcade::SpriteCostume costume = game::COSTUMES.at(costumeId);
    sprites.push_back({{x, y}, costume});
}

static void parseItems(
double x, double y, char c, std::vector<std::shared_ptr<game::Entity>> &items)
{
    std::shared_ptr<game::Entity> ptr;

    if (game::MAP_ITEMS.find(c) == game::MAP_ITEMS.end())
        return;
    ptr = std::make_shared<game::Entity>();
    ptr->pos = {x, y};
    ptr->type = game::MAP_ITEMS.at(c);
    game::Costume costume = static_cast<game::Costume>(game::MAP_ITEMS.at(c));
    items.push_back(ptr);
}

void game::Entity::move()
{
    switch (direction)
    {
        case (UP):
        {
            pos.y -= speed;
            break;
        }
        case (DOWN):
        {
            pos.y += speed;
            break;
        }
        case (LEFT):
        {
            pos.x -= speed;
            break;
        }
        case (RIGHT):
        {
            pos.x += speed;
            break;
        }
    }
}

game::PacmanGame::PacmanGame()
{
    _restartGame();
}

void game::PacmanGame::_restartGame()
{
    _score = 0;
    _level = 0;
    _lives = 0;
    _alive = true;
    _pacman = std::make_shared<Entity>();
    _nextLevel();
}

void game::PacmanGame::_initGhosts()
{
    std::shared_ptr<Ghost> ptr;

    for (std::size_t i = 0; i < GHOST_NB; i++) {
        ptr = std::make_shared<Ghost>();
        ptr->pos = {static_cast<double>(i) + 1, 1};
        ptr->type = GHOST;
        ptr->direction = RIGHT;
        ptr->speed = GHOST_SPEED;
        switch (i % 4)
        {
            case (0):
                ptr->behaviour = RANDOM;
                break;
            case (1):
                ptr->behaviour = PATROL;
                break;
            case (2):
                ptr->behaviour = SEMI;
                break;
            case (3):
                ptr->behaviour = HEATSEAKING;
                break;
        }
        _ghosts.push_back(std::shared_ptr<Ghost>(ptr));
    }
}

const std::vector<arcade::Spritesheet> &
game::PacmanGame::getSpritesheets() const
{
    return SPRITESHEETS;
}

void game::PacmanGame::compute(
const std::chrono::duration<double> &deltaTime, arcade::IContext &context)
{
    _moveClock += deltaTime;
    if (_lives < 0 && _alive) {
        context.saveScore(_score);
        _alive = false;
    }
    if (_alive) {
        while (_canMove()) {
            _moveEntities();
        }
        _determineAllCostume();
    }
}

void game::PacmanGame::handleInput(const arcade::KeyInput &key)
{
    if (key.mapping == arcade::RESTART)
        return _restartGame();

    switch (key.mapping)
    {
        case (arcade::UP):
        {
            _directionRequest = UP;
            _requestedDirection = true;
            break;
        }
        case (arcade::DOWN):
        {
            _directionRequest = DOWN;
            _requestedDirection = true;
            break;
        }
        case (arcade::LEFT):
        {
            _directionRequest = LEFT;
            _requestedDirection = true;
            break;
        }
        case (arcade::RIGHT):
        {
            _directionRequest = RIGHT;
            _requestedDirection = true;
            break;
        }
        default:
            break;
    }
}

void game::PacmanGame::_determinePacmanCostume(void)
{
    switch (_direction) {
        case (UP):
        {
            _pacman->costume = PACMAN_UP;
            break;
        }
        case (DOWN):
        {
            _pacman->costume = PACMAN_DOWN;
            break;
        }
        case (LEFT):
        {
            _pacman->costume = PACMAN_LEFT;
            break;
        }
        case (RIGHT):
        {
            _pacman->costume = PACMAN_RIGHT;
            break;
        }
    }

}

void game::PacmanGame::_determineGhostsCostume(void)
{
    for (std::size_t i = 0; i < _ghosts.size(); i++) {
        switch (_ghosts[i]->behaviour)
        {
            case (RANDOM):
                _ghosts[i]->costume = GHOST_1;
                break;
            case (PATROL):
                _ghosts[i]->costume = GHOST_2;
                break;
            case (SEMI):
                _ghosts[i]->costume = GHOST_3;
                break;
            case (HEATSEAKING):
                _ghosts[i]->costume = GHOST_4;
                break;
        }
    }
}

void game::PacmanGame::_determineAllCostume(void)
{
    _determinePacmanCostume();
    _determineGhostsCostume();
}

void game::PacmanGame::render(arcade::IContext &context)
{
    for (auto &elem : _background) {
        context.drawSprite(elem.costume, elem.pos.x + BOARD_WOFFSET, elem.pos.y + BOARD_HOFFSET);
    }
    _displayInfos(context);
    _displayItems(context);
    _displayPacman(context);
    _displayGhosts(context);
    if (!_alive) {
        context.drawText({"Game Over", 7, 0}, 11, 10);
        if (_lives < 0)
            context.drawText({"No mores lives", 7, 0}, 8, 11);
        context.drawText({"Press <RESTART> to restart", 7, 0}, 7, 12);
        return;
    }
}

void game::PacmanGame::_nextLevel()
{
    _level++;
    _moveClock.zero();
    _background.clear();
    _items.clear();
    _ghosts.clear();
    _resetPacman();
    _resetGhosts();
    _resetItems();
}

void game::PacmanGame::_resetPacman()
{
    _pacman->speed = PACMAN_SPEED;
    _pacman->pos = {9, 17};
}

void game::PacmanGame::_resetGhosts()
{
    _initGhosts();
    for (std::size_t i = 0; i < _ghosts.size(); i++)
        _ghosts[i]->pos = {1, 1};
}

void game::PacmanGame::_resetItems()
{
    std::string path;

    path = MAP_PATH + "/level_" + std::to_string(_level);
    _background = loadMap<SpriteElem>(path + ".txt", parseMap);
    _items = loadMap<std::shared_ptr<game::Entity>>(path + "_items.txt", parseItems);
}


game::PacmanGame::~PacmanGame()
{
}

bool game::PacmanGame::_noDirectObstacle(Direction sens)
{
    Coords coord = _pacman->pos;

    if ((sens == UP || sens == DOWN) && !is_integer(_pacman->pos.y))
        return (true);
    if ((sens == LEFT || sens == RIGHT) && !is_integer(_pacman->pos.x))
        return (true);
    coord.x = static_cast<int>(coord.x);
    coord.y = static_cast<int>(coord.y);
    if (sens == LEFT)
        coord.x -= 1;
    if (sens == RIGHT)
        coord.x += 1;
    if (sens == UP)
        coord.y -= 1;
    if (sens == DOWN)
        coord.y += 1;
    return (!_backgroundOnSpot(COSTUMES.at(WALL), coord));
}

bool game::PacmanGame::_itemsOnSpot(EntityType type, Coords coords)
{
    for (std::size_t i = 0; i < _items.size(); i++) {
        if (_items[i]->type == type && coords == _items[i]->pos)
            return (true);
    }
    return (false);
}

bool game::PacmanGame::_ghostOnSpot(Coords coords)
{
    for (std::size_t i = 0; i < _ghosts.size(); i++) {
        if (samePos(coords, _ghosts[i]->pos))
            return (true);
    }
    return (false);
}

void game::PacmanGame::_removeItem(EntityType type, Coords coords)
{
    for (std::size_t i = 0; i < _items.size(); i++) {
        if (_items[i]->type == type && coords == _items[i]->pos) {
            _items.erase(_items.begin() + i);
        }
    }
}

void _determineAllCostume(void)
{
    // Determine ghosts costume and pacman costume
}

int game::PacmanGame::_determineDirection()
{
    if ((_directionRequest == UP || _directionRequest == DOWN) && !is_integer(_pacman->pos.x))
        return (0);
    if ((_directionRequest == LEFT || _directionRequest == RIGHT) && !is_integer(_pacman->pos.y))
        return (0);
    if (_requestedDirection && _noDirectObstacle(_directionRequest)) {
        _direction = _directionRequest;
        _requestedDirection = false;
    } else {
        return (0);
    }
    return (1);
}

void game::PacmanGame::_determineGhostDirection(std::shared_ptr<Ghost> ghost)
{
    std::vector<Direction> base;
    std::vector<Direction> direct;
    Coords target = _pacman->pos;

    if (!(is_integer(ghost->pos.x) && is_integer(ghost->pos.y)))
        return;
    _getAllButInverse(base, ghost->direction);
    _getAvailableDirect(base, ghost->pos);
    direct = base;
    switch (ghost->behaviour)
    {
        case (RANDOM):
        {
            break;
        }
        case (PATROL):
        {
            for (std::size_t i = 0; i < direct.size(); i++) {
                if (direct[i] == UP && target.y > ghost->pos.y && std::abs(target.y - ghost->pos.y) < 4)
                    direct.erase(direct.begin() + i);
                if (direct[i] == DOWN && target.y < ghost->pos.y && std::abs(target.y - ghost->pos.y) < 4)
                    direct.erase(direct.begin() + i);
                if (direct[i] == LEFT && target.x > ghost->pos.x && std::abs(target.x - ghost->pos.x) < 4)
                    direct.erase(direct.begin() + i);
                if (direct[i] == RIGHT && target.x < ghost->pos.x && std::abs(target.x - ghost->pos.x) < 4)
                    direct.erase(direct.begin() + i);
            }
            break;
        }
        case (SEMI):
        {
            for (std::size_t i = 0; i < direct.size(); i++) {
                if (direct[i] == UP && target.y > ghost->pos.y && rand() % 100 > 20)
                    direct.erase(direct.begin() + i);
                if (direct[i] == DOWN && target.y < ghost->pos.y && rand() % 100 > 20)
                    direct.erase(direct.begin() + i);
                if (direct[i] == LEFT && target.x > ghost->pos.x && rand() % 100 > 20)
                    direct.erase(direct.begin() + i);
                if (direct[i] == RIGHT && target.x < ghost->pos.x && rand() % 100 > 20)
                    direct.erase(direct.begin() + i);
            }
            break;
        }
        case (HEATSEAKING):
        {
            for (std::size_t i = 0; i < direct.size(); i++) {
                if (direct[i] == UP && target.y > ghost->pos.y)
                    direct.erase(direct.begin() + i);
                if (direct[i] == DOWN && target.y < ghost->pos.y)
                    direct.erase(direct.begin() + i);
                if (direct[i] == LEFT && target.x > ghost->pos.x)
                    direct.erase(direct.begin() + i);
                if (direct[i] == RIGHT && target.x < ghost->pos.x)
                    direct.erase(direct.begin() + i);
            }
            break;
        }
    }
    if (direct.size() <= 0)
       direct = base;
    if (base.size() > 0)
        ghost->direction = direct[rand() % direct.size()];
    else
        ghost->direction = _getInverseDirect(ghost->direction);
}

bool game::PacmanGame::_levelComplete()
{
    for (std::size_t i = 0; i < _items.size(); i++) {
        if (_items[i]->type == FRUIT)
            return (false);
    }
    return (true);
}

void game::PacmanGame::_checkCollision()
{
    Coords pos;

    if (!(is_integer(_pacman->pos.x) && is_integer(_pacman->pos.y)))
        return;
    pos.x = static_cast<int>(_pacman->pos.x);
    pos.y = static_cast<int>(_pacman->pos.y);
    if (_itemsOnSpot(FRUIT, pos)) {
        _score += 10;
        _removeItem(FRUIT, pos);
    }
}

void game::PacmanGame::_movePacman(void)
{
    _pacman->direction = _direction;
    if (_noDirectObstacle(_pacman->direction))
        _pacman->move();
    _checkCollision();
}

void game::PacmanGame::_moveGhosts(void)
{
    for (std::size_t i = 0; i < _ghosts.size(); i++) {
        _determineGhostDirection(_ghosts[i]);
        _ghosts[i]->move();
        if (samePos(_ghosts[i]->pos, _pacman->pos)) {
            _resetGhosts();
            _resetPacman();
            _lives--;
        }
    }
}

void game::PacmanGame::_moveEntities()
{
    _determineDirection();
    _movePacman();
    _moveGhosts();
    if (_levelComplete() && _lives >= 0) {
        _lives++;
        _nextLevel();
    }
}

bool game::PacmanGame::_backgroundOnSpot(
arcade::SpriteCostume model, Coords coords)
{
    for (auto &i : _background) {
        if (i.costume == model && i.pos == coords)
            return (true);
    }
    return (false);
}

void game::PacmanGame::_displayPacman(arcade::IContext &context)
{
    context.drawSprite(COSTUMES.at(_pacman->costume), displayround(_pacman->pos.x) + BOARD_WOFFSET,
        displayround(_pacman->pos.y) + BOARD_HOFFSET);
}

void game::PacmanGame::_displayGhosts(arcade::IContext &context)
{
    for (auto &ghost : _ghosts) {
        context.drawSprite(
        COSTUMES.at(ghost->costume), displayround(ghost->pos.x) + BOARD_WOFFSET,
            displayround(ghost->pos.y) + BOARD_HOFFSET);
    }
}

void game::PacmanGame::_displayItems(arcade::IContext &context)
{
    for (auto &items : _items) {
        context.drawSprite(
        COSTUMES.at(items->costume), items->pos.x + BOARD_WOFFSET, items->pos.y + BOARD_HOFFSET);
    }
}

void game::PacmanGame::_displayInfos(arcade::IContext &context)
{
    context.drawText({"PLAYER1", 7, 0}, 0, 0);
    context.drawText({std::to_string(_score), 7, 0}, 10, 0);
    context.drawText({"LEFT   " + std::to_string(_lives), 7, 0}, 16, 0);
    context.drawText({"LEVEL " + std::to_string(_level), 7, 0}, 7, 23);
}

bool game::PacmanGame::_canMove(void)
{
    if (_moveClock >=
    (TIME_TO_MOVE - std::chrono::duration<double>(0.015 * (1)))) {
        _moveClock -=
        TIME_TO_MOVE - std::chrono::duration<double>(0.015 * (1));
        return true;
    }
    return false;
}

void game::PacmanGame::_getAllButInverse(std::vector<Direction> &direct, Direction direction)
{
    if (direction != UP)
        direct.push_back(DOWN);
    if (direction != DOWN)
        direct.push_back(UP);
    if (direction != RIGHT)
        direct.push_back(LEFT);
    if (direction != LEFT)
        direct.push_back(RIGHT);
}

void game::PacmanGame::_getAvailableDirect(std::vector<Direction> &direct, Coords coord)
{
    Coords pos;

    for (std::size_t i = 0; i < direct.size(); i++) {
        pos.x = static_cast<int>(coord.x);
        pos.y = static_cast<int>(coord.y);
        switch (direct[i])
        {
            case(UP):
                pos.y -= 1;
                break;
            case(DOWN):
                pos.y += 1;
                break;
            case(RIGHT):
                pos.x += 1;
                break;
            case(LEFT):
                pos.x -= 1;
                break;
        }
        if (_backgroundOnSpot(COSTUMES.at(WALL), pos)) {
            direct.erase(direct.begin() + i);
            i--;
        }
    }
}

game::Direction game::PacmanGame::_getInverseDirect(Direction direct)
{
    switch (direct)
    {
        case (UP):
            return (DOWN);
        case (DOWN):
            return (UP);
        case (RIGHT):
            return (LEFT);
        case (LEFT):
            return (RIGHT);
        default:
            return (direct);
    }
}

int game::PacmanGame::_autoTurn()
{
    if (_direction == UP || _direction == DOWN) {
        if (_noDirectObstacle(LEFT) && !_noDirectObstacle(RIGHT)) {
            _directionRequest = LEFT;
            return (1);
        }
        if (!_noDirectObstacle(LEFT) && _noDirectObstacle(RIGHT)) {
            _directionRequest = RIGHT;
            return (1);
        }
    } else if (_direction == LEFT || _direction == RIGHT) {
        if (_noDirectObstacle(UP) && !_noDirectObstacle(DOWN)) {
            _directionRequest = UP;
            return (1);
        }
        if (!_noDirectObstacle(UP) && _noDirectObstacle(DOWN)) {
            _directionRequest = DOWN;
            return (1);
        }
    }
    return (0);
}

extern "C" {
game::IGame *entryPoint()
{
    return new game::PacmanGame;
}
int getType()
{
    return arcade::GAME;
}
std::string getName()
{
    return "Pacman";
}
}
