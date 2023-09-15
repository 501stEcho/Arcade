/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** NCurses.hpp
*/

#include "NCurses.hpp"
#include <cstddef>
#include <curses.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

render::NCursesDisplay::NCursesDisplay()
{
    initscr();
    curs_set(0);
    noecho();
    use_default_colors();
    start_color();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
}

render::NCursesDisplay::~NCursesDisplay()
{
    endwin();
}

void render::NCursesDisplay::_parseSpritesheetLine(
std::size_t id, const std::string &line, std::size_t y)
{
    for (std::size_t i = 0; i < line.size(); i += 4) {
        short bg = line[i] - '0';
        short fg = line[i + 1] - '0';
        std::string str = line.substr(i + 2, 2);
        _loadedSpritesheets[id][y][i / 4] = {bg, fg, str};
    }
}

void render::NCursesDisplay::loadSpritesheets(
const std::vector<arcade::Spritesheet> &spritesheets)
{
    _loadedSpritesheets.clear();
    for (const auto &spritesheet : spritesheets)
        _loadSpritesheet(spritesheet);
}

void render::NCursesDisplay::_loadSpritesheet(
const arcade::Spritesheet &spritesheet)
{
    std::string path = spritesheet.textPath;
    std::ifstream text;
    text.open(path);
    if (!text.is_open())
        throw AssetException("Cannot load spritesheet from " + path);
    std::string line;
    std::size_t y = 0;
    while (std::getline(text, line)) {
        _loadedSpritesheets[spritesheet.id].push_back(
        std::vector<SpriteCostume>(line.size() / 3));
        _parseSpritesheetLine(spritesheet.id, line, y);
        y++;
    }
    text.close();
}

void render::NCursesDisplay::clearWin()
{
    if (_clearClock++ % 3 == 0) {
        clear();
        _clearClock = 1;
    }
}

void render::NCursesDisplay::_usePair(short bg, short fg)
{
    short pairId = bg * 10 + fg;
    init_pair(pairId, fg, bg ?: -1);
    attron(COLOR_PAIR(pairId));
}

void render::NCursesDisplay::drawSprite(
const arcade::SpriteCostume &sprite, double x, double y)
{
    Sprite s = {.pos = {x, y},
    .costume =
    _loadedSpritesheets[sprite.spritesheet][sprite.pos.y][sprite.pos.x]};
    _spritesCache.push_back(s);
}

render::SpriteCostume render::NCursesDisplay::_getSpriteChunk(std::size_t &i)
{
    SpriteCostume costume = _spritesCache[i].costume;

    while (i + 1 < _spritesCache.size() &&
    _spritesCache[i + 1].pos.y == _spritesCache[i].pos.y &&
    _spritesCache[i + 1].pos.x == _spritesCache[i].pos.x + 1) {
        SpriteCostume nextCostume = _spritesCache[i + 1].costume;
        if (nextCostume.colorBackground == costume.colorBackground &&
        nextCostume.colorForeground == costume.colorForeground)
            costume.characters += nextCostume.characters;
        else
            break;
        i++;
    }
    return costume;
}

void render::NCursesDisplay::refreshWin()
{
    std::size_t x, y, i = 0, n = _spritesCache.size();
    while (i < n) {
        x = _spritesCache[i].pos.x * 2;
        y = _spritesCache[i].pos.y;
        SpriteCostume costume = _getSpriteChunk(i);
        _usePair(costume.colorBackground, costume.colorForeground);
        mvprintw(y, x, "%s", costume.characters.c_str());
        i++;
    }
    _usePair(COLOR_BLACK, COLOR_WHITE);
    for (const auto &text : _textCache)
        mvprintw(text.pos.y, text.pos.x * 2, "%s", text.text.content.c_str());
    _spritesCache.clear();
    _textCache.clear();
    refresh();
}

void render::NCursesDisplay::drawText(
const arcade::TextDisplay &text, double x, double y)
{
    Text t = {.pos = {x, y}, .text = text};
    _textCache.push_back(t);
}

std::vector<arcade::Event> render::NCursesDisplay::pollEvents()
{
    std::vector<arcade::Event> events;
    int key = getch();

    if (key == ERR)
        return events;
    if (KEYS_TRANSLATE.find(key) != KEYS_TRANSLATE.end()) {
        arcade::RawKey keyId = KEYS_TRANSLATE.at(key);
        events.push_back({keyId, {42, 42}});
    }
    if (key == KEY_MOUSE) {
        MEVENT event;
        if (getmouse(&event) == OK) {
            events.push_back(
            {event.bstate & BUTTON1_CLICKED ? arcade::MOUSE_LEFT
                                            : arcade::MOUSE_RIGHT,
            {static_cast<double>(event.x) / 2, static_cast<double>(event.y)}});
        }
    }
    return events;
}

extern "C" {
render::IRenderer *entryPoint()
{
    return new render::NCursesDisplay;
}
int getType()
{
    return arcade::GRAPHICAL;
}
std::string getName()
{
    return "Ncurses";
}
}
