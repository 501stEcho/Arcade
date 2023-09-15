/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** Core
*/

#pragma once

#include "LibElem.hpp"
#include "context/RenderContext.hpp"
#include "libdl/Archive.hpp"
#include "menu/Menu.hpp"
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <map>
#include <sys/stat.h>

class Core
{
public:
    Core();
    ~Core() = default;
    std::vector<std::string> getDirectorisFiles(std::string path);
    void getAllLib();
    void init(std::string path);
    void addElem(arcade::LibType type, Archive arch, std::string name);
    game::IGame *getActiveGame();
    render::IRenderer *getActiveRender();
    void initKeyMap();
    void setActiveRender(std::shared_ptr<render::IRenderer> render);
    void setActiveGame(std::shared_ptr<game::IGame> game);
    void parseEvent(const arcade::KeyInput &event);
    void loadGame(Archive arch);
    void loadLib(Archive arch);
    void handleEvents();
    void setExit(bool status);
    void loadGameState();
    bool updateGameState(std::chrono::duration<double> deltaTime);
    bool clickOnMenu(const arcade::KeyInput &event);
    arcade::RenderContext _context;
    class CoreError : std::exception
    {
    public:
        CoreError(std::string msg) : _msg(msg){};
        const char *what() const noexcept
        {
            return _msg.data();
        }

    private:
        std::string _msg;
    };

protected:
private:
    std::vector<libElem> _games;
    std::vector<libElem> _renders;
    std::shared_ptr<game::IGame> _activeGame;
    std::shared_ptr<render::IRenderer> _activeRender;
    std::map<arcade::RawKey, arcade::Keymaps> _keyMap;
    std::shared_ptr<game::Menu> _home;
    std::string _userName = "AAA";
    bool _inMenu = true;
    bool _exit = false;
    bool _inPause = false;
    std::chrono::duration<double> _timeReminder = std::chrono::duration<double>::zero();
    int _libIdx = 0;
    int _gameIdx = 0;
};
