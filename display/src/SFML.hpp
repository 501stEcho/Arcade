/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** NCurses.hpp
*/

#include "../../interface/IRenderer.hpp"
#include "../AssetException.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <map>
#include <vector>

namespace render
{
#define SPRITE_SIZE 29
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
    struct LoadedSpritesheet {
        Position dims;
        sf::Texture texture;
    };

    const std::map<sf::Keyboard::Key, arcade::RawKey> KEYS_TRANSLATE = {
    {sf::Keyboard::Key::A, arcade::RawKey::K_A},
    {sf::Keyboard::Key::B, arcade::RawKey::K_B},
    {sf::Keyboard::Key::C, arcade::RawKey::K_C},
    {sf::Keyboard::Key::D, arcade::RawKey::K_D},
    {sf::Keyboard::Key::E, arcade::RawKey::K_E},
    {sf::Keyboard::Key::F, arcade::RawKey::K_F},
    {sf::Keyboard::Key::G, arcade::RawKey::K_G},
    {sf::Keyboard::Key::H, arcade::RawKey::K_H},
    {sf::Keyboard::Key::I, arcade::RawKey::K_I},
    {sf::Keyboard::Key::J, arcade::RawKey::K_J},
    {sf::Keyboard::Key::K, arcade::RawKey::K_K},
    {sf::Keyboard::Key::L, arcade::RawKey::K_L},
    {sf::Keyboard::Key::M, arcade::RawKey::K_M},
    {sf::Keyboard::Key::N, arcade::RawKey::K_N},
    {sf::Keyboard::Key::O, arcade::RawKey::K_O},
    {sf::Keyboard::Key::P, arcade::RawKey::K_P},
    {sf::Keyboard::Key::Q, arcade::RawKey::K_Q},
    {sf::Keyboard::Key::R, arcade::RawKey::K_R},
    {sf::Keyboard::Key::S, arcade::RawKey::K_S},
    {sf::Keyboard::Key::T, arcade::RawKey::K_T},
    {sf::Keyboard::Key::U, arcade::RawKey::K_U},
    {sf::Keyboard::Key::V, arcade::RawKey::K_V},
    {sf::Keyboard::Key::W, arcade::RawKey::K_W},
    {sf::Keyboard::Key::X, arcade::RawKey::K_X},
    {sf::Keyboard::Key::Y, arcade::RawKey::K_Y},
    {sf::Keyboard::Key::Z, arcade::RawKey::K_Z},
    {sf::Keyboard::Key::Num0, arcade::RawKey::K_0},
    {sf::Keyboard::Key::Num1, arcade::RawKey::K_1},
    {sf::Keyboard::Key::Num2, arcade::RawKey::K_2},
    {sf::Keyboard::Key::Num3, arcade::RawKey::K_3},
    {sf::Keyboard::Key::Num4, arcade::RawKey::K_4},
    {sf::Keyboard::Key::Num5, arcade::RawKey::K_5},
    {sf::Keyboard::Key::Num6, arcade::RawKey::K_6},
    {sf::Keyboard::Key::Num7, arcade::RawKey::K_7},
    {sf::Keyboard::Key::Num8, arcade::RawKey::K_8},
    {sf::Keyboard::Key::Num9, arcade::RawKey::K_9},
    {sf::Keyboard::Key::Numpad0, arcade::RawKey::K_0},
    {sf::Keyboard::Key::Numpad1, arcade::RawKey::K_1},
    {sf::Keyboard::Key::Numpad2, arcade::RawKey::K_2},
    {sf::Keyboard::Key::Numpad3, arcade::RawKey::K_3},
    {sf::Keyboard::Key::Numpad4, arcade::RawKey::K_4},
    {sf::Keyboard::Key::Numpad5, arcade::RawKey::K_5},
    {sf::Keyboard::Key::Numpad6, arcade::RawKey::K_6},
    {sf::Keyboard::Key::Numpad7, arcade::RawKey::K_7},
    {sf::Keyboard::Key::Numpad8, arcade::RawKey::K_8},
    {sf::Keyboard::Key::Numpad9, arcade::RawKey::K_9},
    {sf::Keyboard::Key::Space, arcade::RawKey::K_SPACE},
    {sf::Keyboard::Key::Escape, arcade::RawKey::K_ESCAPE},
    {sf::Keyboard::Key::Enter, arcade::RawKey::K_ENTER},
    {sf::Keyboard::Key::Backspace, arcade::RawKey::K_BACKSPACE},
    {sf::Keyboard::Key::Up, arcade::RawKey::K_UP},
    {sf::Keyboard::Key::Down, arcade::RawKey::K_DOWN},
    {sf::Keyboard::Key::Left, arcade::RawKey::K_LEFT},
    {sf::Keyboard::Key::Right, arcade::RawKey::K_RIGHT},
    };

    class SFMLDisplay : public IRenderer
    {
    public:
        SFMLDisplay();
        ~SFMLDisplay();
        void loadSpritesheets(
        const std::vector<arcade::Spritesheet> &) override;
        std::vector<arcade::Event> pollEvents() override;
        void clearWin() override;
        void refreshWin() override;
        void drawSprite(
        const arcade::SpriteCostume &, double, double) override;
        void drawText(const arcade::TextDisplay &, double, double) override;

    protected:
        void _loadSpritesheet(const arcade::Spritesheet &spritesheet);

    private:
        sf::RenderWindow _window;
        std::map<std::size_t, LoadedSpritesheet> _spritesheets;
        sf::Font _font;
    };
}