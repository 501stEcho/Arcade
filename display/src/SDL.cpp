/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** NCurses.hpp
*/

#include "SDL.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_video.h>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

render::SDLDisplay::SDLDisplay()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    _window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(_renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    _font = TTF_OpenFont("./assets/fonts/VeraMono.ttf", 24);
    if (!_font)
        throw AssetException("Cannot load font at ./assets/fonts/VeraMono.ttf");
}

render::SDLDisplay::~SDLDisplay()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    TTF_CloseFont(_font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void render::SDLDisplay::_loadSpritesheet(
const arcade::Spritesheet &spritesheet)
{
    std::string path = spritesheet.graphicPath;
    SDL_Texture *texture = nullptr;
    SDL_Surface *surface = IMG_Load(path.c_str());

    if (!surface)
        throw AssetException("Cannot load spritesheet from " + path);
    texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
        throw AssetException("Cannot create texture from spritesheet at " + path);
    _spritesheets[spritesheet.id] = {spritesheet.unitDims, texture};
}

void render::SDLDisplay::loadSpritesheets(
const std::vector<arcade::Spritesheet> &spritesheets)
{
    _spritesheets.clear();
    for (const auto &spritesheet : spritesheets)
        _loadSpritesheet(spritesheet);
}

void render::SDLDisplay::clearWin()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

SDL_Rect render::SDLDisplay::_getRect(
std::size_t x, std::size_t y, std::size_t w, std::size_t h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

void render::SDLDisplay::drawSprite(
const arcade::SpriteCostume &sprite, double x, double y)
{
    LoadedSpritesheet &spritesheet = _spritesheets[sprite.spritesheet];
    SDL_Texture *texture = spritesheet.texture;
    const Position &idx = sprite.pos;
    const Position &dims = spritesheet.dims;
    SDL_Rect srcRect =
    _getRect(idx.x * dims.x, idx.y * dims.y, dims.x, dims.y);
    SDL_Rect dstRect =
    _getRect(x * SPRITE_SIZE, y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE);

    SDL_RenderCopy(_renderer, texture, &srcRect, &dstRect);
}

void render::SDLDisplay::drawText(
const arcade::TextDisplay &text, double x, double y)
{
    SDL_Color colour = {255, 255, 255, 255};
    SDL_Surface *surface =
    TTF_RenderText_Blended(_font, text.content.c_str(), colour);
    if (!surface)
        return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    SDL_Rect srcRect = _getRect(0, 0, surface->w, surface->h);
    SDL_Rect dstRect =
    _getRect(x * SPRITE_SIZE, y * SPRITE_SIZE, surface->w, surface->h);
    SDL_RenderCopy(_renderer, texture, &srcRect, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render::SDLDisplay::refreshWin()
{
    SDL_RenderPresent(_renderer);
}

std::vector<arcade::Event> render::SDLDisplay::pollEvents()
{
    std::vector<arcade::Event> events;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            if (KEYS_TRANSLATE.find(event.key.keysym.sym) !=
            KEYS_TRANSLATE.end()) {
                arcade::RawKey keyId = KEYS_TRANSLATE.at(event.key.keysym.sym);
                events.push_back({keyId, {42, 42}});
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            SDL_Point position;
            SDL_GetMouseState(&position.x, &position.y);
            events.push_back(
            {event.button.button == SDL_BUTTON_LEFT ? arcade::MOUSE_LEFT
                                                    : arcade::MOUSE_RIGHT,
            {static_cast<double>(position.x) / SPRITE_SIZE,
            static_cast<double>(position.y) / SPRITE_SIZE}});
        }
    }
    return events;
}

extern "C" {
render::IRenderer *entryPoint()
{
    return new render::SDLDisplay;
}
int getType()
{
    return arcade::GRAPHICAL;
}
std::string getName()
{
    return "SDL2";
}
}
