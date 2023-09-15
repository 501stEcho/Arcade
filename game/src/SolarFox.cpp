/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** Snake.hpp
*/

#include "SolarFox.hpp"
#include <utility>

std::shared_ptr<game::Entity> game::initItem(ItemType type, Coords pos)
{
    std::shared_ptr<Item> nitem = std::make_shared<Item>();

    nitem->type = ITEM;
    nitem->itemtype = type;
    nitem->speed = 0;
    nitem->direction = RIGHT;
    nitem->pos = pos;
    nitem->state = INTACT;
    switch (type)
    {
        case (COIN):
            nitem->costume = COIN_BASE;
            break;
        case (SPECICOIN):
            nitem->costume = COIN_LV2;
            break;
        case (BOMB):
            nitem->costume = MINE;
            break;
    }
    return (nitem);
}

bool game::samePos(Coords pos1, Coords pos2)
{
    if (static_cast<int>(pos1.x) == static_cast<int>(pos2.x)) {
        if (static_cast<int>(pos1.y) == static_cast<int>(pos2.y)) {
            return (true);
        }
    }
    return (false);
}

game::SolarFoxGame::SolarFoxGame()
{
    _score = 0;
    _level = 0;
    _lives = 0;
    _alive = true;
    srand(time(NULL));
    _nextLevel();
}

void game::SolarFoxGame::_restartGame()
{
    _score = 0;
    _level = 0;
    _lives = 0;
    _alive = true;
    _nextLevel();
}

const std::vector<arcade::Spritesheet> &game::SolarFoxGame::getSpritesheets() const
{
    return SPRITESHEETS;
}

void game::Ship::move(Direction ndirection)
{
    if (state == DEAD || state == STUNNED)
        return;
    if (faction == ALLY) {
        direction = ndirection;
    } else {
        switch (direction) {
            case (UP):
                if (pos.y <= 0)
                    direction = DOWN;
                break;
            case (DOWN):
                if (pos.y >= BOARD_HEIGHT - 1)
                    direction = UP;
                break;
            case (LEFT):
                if (pos.x <= 0)
                    direction = RIGHT;
                break;
            case (RIGHT):
                if (pos.x >= BOARD_WIDTH - 1)
                    direction = LEFT;
                break;
        }
    }
    _applySpeed(*this);
}

bool game::Ship::display()
{
    return (true);
}

bool game::Ship::exists()
{
    return (true);
}

game::GameState game::Ship::winLose()
{
    switch (faction)
    {
        case (ALLY):
            if (state == DEAD)
                return (LOSS);
            else if (pos.x >= BOARD_WIDTH || pos.y >= BOARD_HEIGHT || pos.x < 0 || pos.y < 0)
                return (LOSS);
            else
                return (UNKNOWN);
            break;
        case (FOE):
            return (UNKNOWN);
            break;
    }
    return (UNKNOWN);
}

std::string game::Ship::getMessage()
{
    switch (faction)
    {
        case (ALLY):
            if (state == DEAD)
                return deathmsg;
            else
                return ("");
            break;
        case (FOE):
            return ("");
            break;
    }
    return ("");
}

void game::Ship::setCostume()
{
    if (faction == FOE) {
        if (direction == UP || direction == DOWN) {
            if (pos.x < BOARD_WIDTH / 2)
                costume = ENNEMY_RIGHT_1;
            else
                costume = ENNEMY_LEFT_1;
        } else {
            if (pos.y < BOARD_HEIGHT / 2)
                costume = ENNEMY_DOWN_1;
            else
                costume = ENNEMY_UP_1;
        }
    } else {
        switch (direction) {
            case (UP):
                costume = costume;
                break;
            case (DOWN):
                costume = costume;
                break;
            case (LEFT):
                costume = SHIP_LEFT_1;
                break;
            case (RIGHT):
                costume = SHIP_RIGHT_1;
                break;
        }
    }
}

void game::Ship::interact(std::vector<std::shared_ptr<Entity>> &env)
{
    switch (faction) {
        case (FOE) :
            maybeShoot(env);
            break;
        case (ALLY) :
            break;
    }
}

void game::Ship::maybeShoot(std::vector<std::shared_ptr<Entity>> &env)
{
    std::shared_ptr<Bullet> bul;

    if (rand() % 1000 > 10)
        return;
    bul = std::make_shared<Bullet>();
    bul->pos = pos;
    bul->costume = FOELASER;
    bul->faction = FOE;
    bul->type = BULLET;
    bul->speed = ALLY_SPEED * 2;
    if (direction == UP || direction == DOWN) {
        if (pos.x < BOARD_WIDTH / 2) {
            bul->direction = RIGHT;
            bul->range = BOARD_WIDTH;
        } else {
            bul->range = BOARD_WIDTH;
            bul->direction = LEFT;
        }
    } else {
        if (pos.y < BOARD_HEIGHT / 2) {
            bul->range = BOARD_HEIGHT;
            bul->direction = DOWN;
        } else {
            bul->range = BOARD_HEIGHT;
            bul->direction = UP;
        }
    }
    env.push_back(bul);
}

void game::Ship::shoot(std::vector<std::shared_ptr<Entity>> &env)
{
    std::shared_ptr<Bullet> bul;

    bul = std::make_shared<Bullet>();
    bul->pos = pos;
    bul->costume = ALLYLASER;
    bul->faction = ALLY;
    bul->type = BULLET;
    bul->speed = ALLY_SPEED * 3;
    bul->direction = direction;
    bul->range = 2;
    env.push_back(bul);
}

void game::Ship::handleEnv(std::vector<std::shared_ptr<Entity>> &env)
{
    for (std::size_t i = 0; i < env.size(); i++) {
        if (samePos(env[i]->pos, pos)) {
            switch (env[i]->type) {
                case (SHIP):
                    handleCollision(std::dynamic_pointer_cast<Ship>(env[i]));
                    break;
                case (BULLET):
                    handleCollision(std::dynamic_pointer_cast<Bullet>(env[i]));
                    break;
                case (ITEM):
                    handleCollision(std::dynamic_pointer_cast<Item>(env[i]));
                    break;
            }
        }
    }
}

void game::Ship::handleInput(const arcade::KeyInput &key, std::vector<std::shared_ptr<Entity>> &env)
{
    switch (faction) {
        case (ALLY):
            if (key.mapping == arcade::ACTIONA) {
                shoot(env);
            }
            break;

        case (FOE):
            break;
    }
}

void game::Ship::handleCollision(std::shared_ptr<Ship>)
{
    return;
}

void game::Ship::handleCollision(std::shared_ptr<Bullet> other)
{
    if (faction == ALLY && other->faction == FOE) {
        state = DEAD;
    }
}

void game::Ship::handleCollision(std::shared_ptr<Item> other)
{
    if (faction == ALLY && other->itemtype == BOMB) {
        state = DEAD;
    }
}

void game::Bullet::move(Direction)
{
    _applySpeed(*this);
    range -= speed;
}

bool game::Bullet::display()
{
    if (range <= 0)
        return false;
    return true;
}

bool game::Bullet::exists()
{
    if (range <= 0)
        return (false);
    return (true);
}

game::GameState game::Bullet::winLose()
{
    return (UNKNOWN);
}

std::string game::Bullet::getMessage()
{
    return ("");
}

void game::Bullet::setCostume()
{
    switch (faction)
    {
        case (ALLY):
            costume = ALLYLASER;
            break;
        case (FOE):
            costume = FOELASER;
            break;
    }
}

void game::Bullet::interact(std::vector<std::shared_ptr<Entity>> &)
{
}

void game::Bullet::handleEnv(std::vector<std::shared_ptr<Entity>> &env)
{
    for (std::size_t i = 0; i < env.size(); i++) {
        if (samePos(env[i]->pos, pos)) {
            switch (env[i]->type) {
                case (SHIP):
                    handleCollision(std::dynamic_pointer_cast<Ship>(env[i]));
                    break;
                case (BULLET):
                    handleCollision(std::dynamic_pointer_cast<Bullet>(env[i]));
                    break;
                case (ITEM):
                    handleCollision(std::dynamic_pointer_cast<Item>(env[i]));
                    break;
            }
        }
    }
}

void game::Bullet::handleInput(const arcade::KeyInput &, std::vector<std::shared_ptr<Entity>> &)
{
    return;
}

void game::Bullet::handleCollision(std::shared_ptr<Ship> other)
{
    if (faction != other->faction) {
        range = 0;
    }
}

void game::Bullet::handleCollision(std::shared_ptr<Bullet> other)
{
    if (faction != other->faction)
        range = 0;
}

void game::Bullet::handleCollision(std::shared_ptr<Item>)
{
    switch (faction) {
        case (FOE):
            break;
        case (ALLY):
            range = 0;
            break;
    }
}


void game::Item::move(Direction)
{
    _applySpeed(*this);
}

bool game::Item::display()
{
    if (state == CONSUMED)
        return (false);
    return (true);
}

bool game::Item::exists()
{
    if (state == CONSUMED)
        return (false);
    return (true);
}

game::GameState game::Item::winLose()
{
    return (UNKNOWN);
}

std::string game::Item::getMessage()
{
    return ("");
}

void game::Item::setCostume()
{

}

void game::Item::interact(std::vector<std::shared_ptr<Entity>> &)
{

}

void game::Item::handleEnv(std::vector<std::shared_ptr<Entity>> &env)
{
    for (std::size_t i = 0; i < env.size(); i++) {
        if (samePos(env[i]->pos, pos)) {
            switch (env[i]->type) {
                case (SHIP):
                    handleCollision(std::dynamic_pointer_cast<Ship>(env[i]));
                    break;
                case (BULLET):
                    handleCollision(std::dynamic_pointer_cast<Bullet>(env[i]));
                    break;
                case (ITEM):
                    handleCollision(std::dynamic_pointer_cast<Item>(env[i]));
                    break;
            }
        }
    }
}

void game::Item::handleInput(const arcade::KeyInput &, std::vector<std::shared_ptr<Entity>> &)
{
    return;
}

void game::Item::handleCollision(std::shared_ptr<Ship> other)
{
    if (other->faction == FOE)
        return;
    switch (itemtype)
    {
        case (COIN):
            break;
        case (SPECICOIN):
            break;
        case (BOMB):
            state = CONSUMED;
            break;
    }
}

void game::Item::handleCollision(std::shared_ptr<Bullet> other)
{
    if (other->faction == FOE)
        return;
    switch (itemtype)
    {
        case (COIN):
            state = CONSUMED;
            break;
        case (SPECICOIN):
            costume = COIN_BASE;
            itemtype = COIN;
            break;
        case (BOMB):
            state = CONSUMED;
            break;
    }
}

void game::Item::handleCollision(std::shared_ptr<Item>)
{

}


void game::SolarFoxGame::compute(const std::chrono::duration<double>& dtime, arcade::IContext &context)
{
    _moveClock += dtime;
    _interactClock += dtime;
    if (_alive) {
        while (_canMove())
            _moveItems();
        while (_canInteract())
            _interactItems();
        for (std::size_t i = 0; i < _items.size();i++)
            _items[i]->handleEnv(_items);
        _cleanItems();
        _checkGameState();
    } else {
        if (!_saved)
            context.saveScore(_score);
        _saved = true;
    }
}

bool game::SolarFoxGame::_canMove(void)
{
    if (_moveClock >= TIME_TO_MOVE) {
        _moveClock -= TIME_TO_MOVE;
        return true;
    }
    return false;
}

bool game::SolarFoxGame::_canInteract(void)
{
    if (_interactClock >= TIME_TO_INTERACT) {
        _interactClock -= TIME_TO_INTERACT;
        return true;
    }
    return false;
}

void game::SolarFoxGame::handleInput(const arcade::KeyInput &key)
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
    for (std::size_t i = 0; i < _items.size(); i++)
        _items[i]->handleInput(key, _items);
}

void game::SolarFoxGame::render(arcade::IContext &context)
{
    for (auto &elem : _background) {
        context.drawSprite(elem.costume, elem.pos.x + BOARD_WOFFSET, elem.pos.y + BOARD_HOFFSET);
    }
    _displayInfos(context);
    _displayItems(context);
    if (!_alive) {
        context.drawText({"Game Over", 7, 0}, 5, 10);
        context.drawText({"Press <RESTART> to restart", 7, 0}, 3, 12);
        return;
    }
}

void game::SolarFoxGame::_nextLevel()
{
    _level++;
    _moveClock = std::chrono::duration<double>::zero();
    _interactClock = std::chrono::duration<double>::zero();
    _background.clear();
    _items.clear();
    _resetBoard();
    _resetShips();
    _resetCoins();
}

game::SolarFoxGame::~SolarFoxGame()
{
}

bool game::SolarFoxGame::_levelComplete()
{
    return (true);
}

void game::_applySpeed(Entity &mob)
{
    switch (mob.direction)
    {
        case (UP):
        {
            mob.pos.y -= mob.speed;
            break;
        }
        case (DOWN):
        {
            mob.pos.y += mob.speed;
            break;
        }
        case (LEFT):
        {
            mob.pos.x -= mob.speed;
            break;
        }
        case (RIGHT):
        {
            mob.pos.x += mob.speed;
            break;
        }
    }
}


void game::SolarFoxGame::_handleDirection()
{
    if (_requestedDirection) {
        _direction = _directionRequest;
        _requestedDirection = false;
    }
}

void game::SolarFoxGame::_moveItems()
{
    _handleDirection();
    for (std::size_t i = 0; i < _items.size(); i++) {
        _items[i]->move(_direction);
    }
}

void game::SolarFoxGame::_interactItems()
{
    std::vector<std::shared_ptr<Entity>> nelem;
    for (std::size_t i = 0; i < _items.size(); i++) {
        _items[i]->interact(nelem);
    }
    _items.insert(_items.begin(), nelem.begin(), nelem.end());
}

void game::SolarFoxGame::_cleanItems()
{
    for (std::size_t i = 0; i < _items.size(); i++) {
        if (!(_items[i]->exists())) {
            if (_items[i]->type == ITEM) {
                if (std::dynamic_pointer_cast<Item>(_items[i])->itemtype == BOMB)
                    _score += 100;
                else
                    _score += 50;
            }
            _items.erase(_items.begin() + i);
        }
    }
}

void game::SolarFoxGame::_checkGameState()
{
    for (std::size_t i = 0; i < _items.size(); i++) {
        if (_items[i]->winLose() == LOSS) {

            _alive = false;
        }
        if (_items[i]->type == ITEM) {
            if (std::dynamic_pointer_cast<Item>(_items[i])->itemtype != BOMB)
                return;
        }
    }
    _nextLevel();
}

void game::SolarFoxGame::_resetBoard()
{
    for (double i = 0; i < BOARD_HEIGHT; i += 1) {
        for (double j = 0; j < BOARD_WIDTH; j += 1) {
            if (COSTUMES.find(SKY) != COSTUMES.end())
                _background.push_back({{j, i}, COSTUMES.at(SKY)});
        }
    }
}

void game::SolarFoxGame::_resetShips()
{
    _requestedDirection = false;
    _direction = RIGHT;
    for (std::size_t i = 0; i < 4; i++)
        _items.push_back(_createFoe(i));
    _items.push_back(_createAlly());
}

void game::SolarFoxGame::_resetCoins()
{
    std::size_t temp;

    for (std::size_t i = 0; i < BOARD_HEIGHT; i++) {
        for (std::size_t j = 0; j < BOARD_WIDTH; j++) {
            temp = rand() % 100;
            if (temp > 96)
                _items.push_back(initItem(BOMB, {static_cast<double>(j), static_cast<double>(i)}));
            else if (temp > 78)
                _items.push_back(initItem(COIN, {static_cast<double>(j), static_cast<double>(i)}));
            else if (temp > 74)
                _items.push_back(initItem(SPECICOIN, {static_cast<double>(j), static_cast<double>(i)}));
        }
    }
}

std::shared_ptr<game::Entity> game::SolarFoxGame::_createAlly(void)
{
    std::shared_ptr<Ship> nship = std::make_shared<Ship>();

    nship->faction = ALLY;
    nship->state = ALIVE;
    nship->speed = ALLY_SPEED;
    nship->direction = RIGHT;
    nship->costume = SHIP_RIGHT_1;
    nship->type = SHIP;
    nship->pos = {BOARD_WIDTH / 2, BOARD_HEIGHT / 2};
    return (nship);
}

std::shared_ptr<game::Entity> game::SolarFoxGame::_createFoe(std::size_t id)
{
    std::shared_ptr<Ship> nship = std::make_shared<Ship>();

    nship->faction = FOE;
    nship->speed = FOE_SPEED;
    nship->state = ALIVE;
    nship->type = SHIP;
    switch (id) {
        case (0) :
        {
            nship->direction = UP;
            nship->costume = ENNEMY_RIGHT_1;
            nship->pos = {-1, BOARD_HEIGHT / 2};
            break;
        }
        case (1) :
        {
            nship->direction = RIGHT;
            nship->costume = ENNEMY_DOWN_1;
            nship->pos = {BOARD_WIDTH / 2, -1};
            break;
        }
        case (2) :
        {
            nship->direction = DOWN;
            nship->costume = ENNEMY_LEFT_1;
            nship->pos = {BOARD_WIDTH, BOARD_HEIGHT / 2};
            break;
        }
        case (3) :
        {
            nship->direction = LEFT;
            nship->costume = ENNEMY_UP_1;
            nship->pos = {BOARD_WIDTH / 2, BOARD_HEIGHT};
            break;
        }
    }
    return (nship);
}

void game::SolarFoxGame::_displayItems(arcade::IContext &context)
{
    double x;
    double y;

    for (std::size_t i = 0; i < _items.size(); i++) {
        if (_items[i]->display()) {
            _items[i]->setCostume();
        }
    }
    for (std::size_t i = 0; i < _items.size(); i++) {
        x = _items[i]->pos.x;
        y = _items[i]->pos.y;
        if (_items[i]->type == ITEM)
            context.drawSprite(COSTUMES.at(_items[i]->costume), x + BOARD_WOFFSET, y + BOARD_HOFFSET);
    }
    for (std::size_t i = 0; i < _items.size(); i++) {
        x = _items[i]->pos.x;
        y = _items[i]->pos.y;
        if (_items[i]->type == SHIP)
            context.drawSprite(COSTUMES.at(_items[i]->costume), x + BOARD_WOFFSET - 0.5, y + BOARD_HOFFSET - 0.5);
    }
    for (std::size_t i = 0; i < _items.size(); i++) {
        x = _items[i]->pos.x;
        y = _items[i]->pos.y;
        if (_items[i]->type == BULLET)
            context.drawSprite(COSTUMES.at(_items[i]->costume), x + BOARD_WOFFSET - 0.5, y + BOARD_HOFFSET - 0.5);
    }
}

void game::SolarFoxGame::_displayInfos(arcade::IContext &context)
{
    context.drawText({"PLAYER1", 7, 0}, 0, 0);
    context.drawText({std::to_string(_score), 7, 0}, 5, 0);
    context.drawText({"LEFT   " + std::to_string(_lives), 7, 0}, 8, 0);
    context.drawText({"WAVE " + std::to_string(_level), 7, 0}, 3, 23);
}

extern "C" {
    game::IGame *entryPoint() {
        return new game::SolarFoxGame;
    }
    int getType() {
        return arcade::GAME;
    }
    std::string getName() {
        return "Solar Fox";
    }
}
