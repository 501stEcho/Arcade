/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** NCurses.hpp
*/

#include "../../interface/IRenderer.hpp"
#include "../AssetException.hpp"
#include <cstddef>
#include <map>
#include <ncurses.h>
#include <string>
#include <vector>

namespace render
{
    struct SpriteCostume {
        short colorBackground;
        short colorForeground;
        std::string characters;
    };

    struct Sprite {
        Coords pos;
        SpriteCostume costume;
    };

    struct Text {
        Coords pos;
        arcade::TextDisplay text;
    };

    const std::map<short, arcade::RawKey> KEYS_TRANSLATE = {
    {'a', arcade::RawKey::K_A},
    {'b', arcade::RawKey::K_B},
    {'c', arcade::RawKey::K_C},
    {'d', arcade::RawKey::K_D},
    {'e', arcade::RawKey::K_E},
    {'f', arcade::RawKey::K_F},
    {'g', arcade::RawKey::K_G},
    {'h', arcade::RawKey::K_H},
    {'i', arcade::RawKey::K_I},
    {'j', arcade::RawKey::K_J},
    {'k', arcade::RawKey::K_K},
    {'l', arcade::RawKey::K_L},
    {'m', arcade::RawKey::K_M},
    {'n', arcade::RawKey::K_N},
    {'o', arcade::RawKey::K_O},
    {'p', arcade::RawKey::K_P},
    {'q', arcade::RawKey::K_Q},
    {'r', arcade::RawKey::K_R},
    {'s', arcade::RawKey::K_S},
    {'t', arcade::RawKey::K_T},
    {'u', arcade::RawKey::K_U},
    {'v', arcade::RawKey::K_V},
    {'w', arcade::RawKey::K_W},
    {'x', arcade::RawKey::K_X},
    {'y', arcade::RawKey::K_Y},
    {'z', arcade::RawKey::K_Z},
    {'0', arcade::RawKey::K_0},
    {'1', arcade::RawKey::K_1},
    {'2', arcade::RawKey::K_2},
    {'3', arcade::RawKey::K_3},
    {'4', arcade::RawKey::K_4},
    {'5', arcade::RawKey::K_5},
    {'6', arcade::RawKey::K_6},
    {'7', arcade::RawKey::K_7},
    {'8', arcade::RawKey::K_8},
    {'9', arcade::RawKey::K_9},
    {' ', arcade::RawKey::K_SPACE},
    {27, arcade::RawKey::K_ESCAPE},
    {10, arcade::RawKey::K_ENTER},
    {127, arcade::RawKey::K_BACKSPACE},
    {KEY_UP, arcade::RawKey::K_UP},
    {KEY_DOWN, arcade::RawKey::K_DOWN},
    {KEY_LEFT, arcade::RawKey::K_LEFT},
    {KEY_RIGHT, arcade::RawKey::K_RIGHT},
    {KEY_BACKSPACE, arcade::RawKey::K_BACKSPACE},
    {KEY_DC, arcade::RawKey::K_BACKSPACE},
    };

    class NCursesDisplay : public IRenderer
    {
    public:
        NCursesDisplay();
        ~NCursesDisplay();
        void loadSpritesheets(
        const std::vector<arcade::Spritesheet> &) override;
        std::vector<arcade::Event> pollEvents() override;
        void clearWin() override;
        void refreshWin() override;
        void drawSprite(
        const arcade::SpriteCostume &, double, double) override;
        void drawText(const arcade::TextDisplay &, double, double) override;

    private:
        void _loadSpritesheet(const arcade::Spritesheet &spritesheet);
        void _parseSpritesheetLine(
        std::size_t id, const std::string &line, std::size_t y);
        std::map<std::size_t, matrix<SpriteCostume>> _loadedSpritesheets;
        void _usePair(short bg, short fg);
        SpriteCostume _getSpriteChunk(std::size_t &i);
        std::size_t _clearClock = 0;
        std::vector<Sprite> _spritesCache;
        std::vector<Text> _textCache;
    };
}