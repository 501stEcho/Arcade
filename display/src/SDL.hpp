/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** NCurses.hpp
*/

#include "../../interface/IRenderer.hpp"
#include "../AssetException.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <cstddef>
#include <iostream>
#include <map>
#include <vector>

namespace render
{
#define SPRITE_SIZE 30
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
    struct LoadedSpritesheet {
        Position dims;
        SDL_Texture *texture;
    };

    const std::map<SDL_Keycode, arcade::RawKey> KEYS_TRANSLATE = {
    {SDLK_a, arcade::RawKey::K_A}, {SDLK_b, arcade::RawKey::K_B},
    {SDLK_c, arcade::RawKey::K_C}, {SDLK_d, arcade::RawKey::K_D},
    {SDLK_e, arcade::RawKey::K_E}, {SDLK_f, arcade::RawKey::K_F},
    {SDLK_g, arcade::RawKey::K_G}, {SDLK_h, arcade::RawKey::K_H},
    {SDLK_i, arcade::RawKey::K_I}, {SDLK_j, arcade::RawKey::K_J},
    {SDLK_k, arcade::RawKey::K_K}, {SDLK_l, arcade::RawKey::K_L},
    {SDLK_m, arcade::RawKey::K_M}, {SDLK_n, arcade::RawKey::K_N},
    {SDLK_o, arcade::RawKey::K_O}, {SDLK_p, arcade::RawKey::K_P},
    {SDLK_q, arcade::RawKey::K_Q}, {SDLK_r, arcade::RawKey::K_R},
    {SDLK_s, arcade::RawKey::K_S}, {SDLK_t, arcade::RawKey::K_T},
    {SDLK_u, arcade::RawKey::K_U}, {SDLK_v, arcade::RawKey::K_V},
    {SDLK_w, arcade::RawKey::K_W}, {SDLK_x, arcade::RawKey::K_X},
    {SDLK_y, arcade::RawKey::K_Y}, {SDLK_z, arcade::RawKey::K_Z},
    {SDLK_0, arcade::RawKey::K_0}, {SDLK_1, arcade::RawKey::K_1},
    {SDLK_2, arcade::RawKey::K_2}, {SDLK_3, arcade::RawKey::K_3},
    {SDLK_4, arcade::RawKey::K_4}, {SDLK_5, arcade::RawKey::K_5},
    {SDLK_6, arcade::RawKey::K_6}, {SDLK_7, arcade::RawKey::K_7},
    {SDLK_8, arcade::RawKey::K_8}, {SDLK_9, arcade::RawKey::K_9},
    {SDLK_SPACE, arcade::RawKey::K_SPACE},
    {SDLK_RETURN, arcade::RawKey::K_ENTER},
    {SDLK_ESCAPE, arcade::RawKey::K_ESCAPE}, {SDLK_UP, arcade::RawKey::K_UP},
    {SDLK_DOWN, arcade::RawKey::K_DOWN}, {SDLK_LEFT, arcade::RawKey::K_LEFT},
    {SDLK_RIGHT, arcade::RawKey::K_RIGHT},
    {SDLK_BACKSPACE, arcade::RawKey::K_BACKSPACE}};

    class SDLDisplay : public IRenderer
    {
    public:
        SDLDisplay();
        ~SDLDisplay();
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
        SDL_Rect _getRect(
        std::size_t x, std::size_t y, std::size_t w, std::size_t h);

    private:
        SDL_Window *_window;
        SDL_Renderer *_renderer;
        std::map<std::size_t, LoadedSpritesheet> _spritesheets;
        TTF_Font *_font;
    };
}