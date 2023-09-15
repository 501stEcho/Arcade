/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** Core
*/

#include "Core.hpp"
#include "../Arcade.hpp"
#include <chrono>
#include <iostream>
#include <iterator>
Core::Core()
{
    _inMenu = true;
    initKeyMap();
}

void Core::init(std::string path)
{
    try {
        Archive startupLib(path);
        getAllLib();

        if (startupLib.getType<arcade::LibType>("getType") !=
        arcade::LibType::GRAPHICAL)
            throw CoreError("Only graphical Lib is allow on startup");
        if (_games.size() < 1 || _renders.size() < 1)
            throw CoreError("need 1 game LIB and 1 render LIB");

        std::shared_ptr<game::Menu> game(
        new game::Menu(_renders, _games, _keyMap));
        _home = game;
        _activeGame = game;
        loadLib(startupLib);
    } catch (Archive::ArchError err) {
        throw CoreError(err.what());
    }
}

void Core::initKeyMap()
{
    std::map<arcade::RawKey, arcade::Keymaps> res{
    {arcade::RawKey::K_UP, arcade::Keymaps::UP},
    {arcade::RawKey::K_DOWN, arcade::Keymaps::DOWN},
    {arcade::RawKey::K_LEFT, arcade::Keymaps::LEFT},
    {arcade::RawKey::K_RIGHT, arcade::Keymaps::RIGHT},
    {arcade::RawKey::K_Q, arcade::Keymaps::QUIT},
    {arcade::RawKey::K_R, arcade::Keymaps::RESTART},
    {arcade::RawKey::K_M, arcade::Keymaps::NEXT_GAME},
    {arcade::RawKey::K_L, arcade::Keymaps::NEXT_GRAPH},
    {arcade::RawKey::K_S, arcade::Keymaps::MENU},
    {arcade::RawKey::K_ENTER, arcade::Keymaps::ENTER},
    {arcade::RawKey::K_U, arcade::Keymaps::SUBMENU1},
    {arcade::RawKey::K_P, arcade::Keymaps::PAUSE},
    {arcade::RawKey::K_A, arcade::Keymaps::ACTIONA},
    {arcade::RawKey::K_Z, arcade::Keymaps::ACTIONB},
    {arcade::RawKey::K_I, arcade::Keymaps::SUBMENU2}};

    _keyMap = res;
}

std::vector<std::string> Core::getDirectorisFiles(std::string path)
{
    std::vector<std::string> files;

    const char *dir = path.c_str();

    if (!std::filesystem::exists(dir))
        return files;
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        if (entry.file_size() && (entry.path().c_str()[0] != '.'))
            files.push_back(entry.path());
    }
    return files;
}

game::IGame *Core::getActiveGame()
{
    return _activeGame.get();
}

void Core::setActiveGame(std::shared_ptr<game::IGame> game)
{
    _activeGame = game;
    if (game == NULL)
        return;
    loadGameState();
}

void Core::setActiveRender(std::shared_ptr<render::IRenderer> render)
{
    _activeRender = render;
    _context.setRenderer(render);
}

void Core::addElem(arcade::LibType type, Archive arch, std::string name)
{
    if (type == arcade::LibType::GAME)
        _games.push_back({arch, name});
    if (type == arcade::LibType::GRAPHICAL)
        _renders.push_back({arch, name});
}

void Core::getAllLib()
{
    std::vector<std::string> files = getDirectorisFiles("lib/");

    for (int i = 0; i < files.size(); i++) {
        Archive arch(files[i]);
        arcade::LibType type = arch.getType<arcade::LibType>("getType");
        std::string name = arch.getType<std::string>("getName");
        addElem(type, arch, name);
    }
    return;
}

void Core::loadLib(Archive arch)
{
    if (_exit)
        return;
    try {
        setActiveRender(NULL);
        std::shared_ptr<render::IRenderer> lib(
        arch.getSymbol<render::IRenderer>("entryPoint"));
        setActiveRender(lib);
    } catch (const std::exception &exept) {
        setExit(true);
        std::cerr << exept.what() << std::endl;
    }
}

void Core::setExit(bool status){
    _exit = status;
}

void Core::loadGame(Archive arch)
{
    if (_inMenu)
        return;
    std::shared_ptr<game::IGame> game(
    arch.getSymbol<game::IGame>("entryPoint"));

    _context.setGameName(arch.getType<std::string>("getName"));
    setActiveGame(game);
}

bool isEventInVector(std::vector<arcade::Keymaps> vec, arcade::Keymaps elem)
{
    if (std::find(vec.begin(), vec.end(), elem) != vec.end())
        return true;
    return false;
}

bool Core::clickOnMenu(const arcade::KeyInput &event)
{
    if (event.keyEvent.key != arcade::MOUSE_LEFT)
        return false;
    if (event.keyEvent.pos.x < 1 || event.keyEvent.pos.x > 12)
        return false;
    if (event.keyEvent.pos.y < 21 || event.keyEvent.pos.y > 24)
        return false;
    return true;
}

void Core::loadGameState()
{
    if (_exit)
        return;
    try {
        auto res = _activeGame->getSpritesheets();
        _activeRender->loadSpritesheets(res);
    } catch (const std::exception &exept) {
        setExit(true);
        std::cerr << exept.what() << std::endl;
    }
}

bool Core::updateGameState(std::chrono::duration<double> deltaTime)
{  
    if (_exit)
        return false;
    handleEvents();
    if (!getActiveGame() || _exit)
        return false;
    _timeReminder += deltaTime;
    if (!_inPause)
        _activeGame->compute(deltaTime, _context);

    while (_timeReminder > FRAMERATE) {
        _activeRender->clearWin();
        _activeGame->render(_context);
        if (_inPause)
            _context.drawText({"PAUSE", 7, 0}, 5, 10);
        _activeRender->refreshWin();
        _timeReminder -= FRAMERATE;
    }
    return true;
}

void Core::handleEvents()
{
    std::vector<arcade::Event> keyEvent = _activeRender->pollEvents();

    if (keyEvent.empty())
        return;
    for (auto &input : keyEvent) {
        if (_keyMap.find(input.key) != _keyMap.end())
            parseEvent({input, _keyMap[input.key]});
        else
            parseEvent({input, arcade::Keymaps::NONE});
    }
}

void Core::parseEvent(const arcade::KeyInput &event)
{
    if (!_inMenu && event.keyEvent.key == arcade::RawKey::K_P)
        _inPause = !_inPause;
    if (event.mapping == arcade::Keymaps::MENU) {
        if (!_inMenu) {
            _inMenu = true;
            _inPause = false;
            setActiveGame(_home);
        }
        return;
    }

    if (_inMenu &&
    (event.mapping == arcade::Keymaps::ENTER || clickOnMenu(event))) {
        _inMenu = false;
        _gameIdx = _home->getSelectedGameIdx();
        _libIdx = _home->getSelectedRenderIdx();
        _userName = _home->getUserName();
        _context.setUserName(_userName);
        _keyMap = _home->getUpdatedKeyMap();
        loadLib(_renders[_libIdx].arch);
        loadGame(_games[_gameIdx].arch);
        _context.setGameName(_home->getGameName());
    }

    if (event.mapping == arcade::Keymaps::NEXT_GRAPH) {
        _libIdx = _libIdx + 1 >= _renders.size() ? 0 : _libIdx + 1;
        loadLib(_renders[_libIdx].arch);
        loadGameState();
    }

    if (event.mapping == arcade::Keymaps::NEXT_GAME) {
        _gameIdx = _gameIdx + 1 >= _games.size() ? 0 : _gameIdx + 1;
        loadGame(_games[_gameIdx].arch);
    }

    if (event.mapping == arcade::Keymaps::RESTART) {
        loadGame(_games[_gameIdx].arch);
    }

    if (event.mapping == arcade::Keymaps::QUIT || _exit) {
        if (_inMenu) {
            setActiveGame(NULL);
            setActiveRender(nullptr);
        } else {
            _inMenu = true;
            _inPause = false;
            setActiveGame(_home);
        }
        return;
    }
    if (!_exit)
        _activeGame->handleInput(event);
}