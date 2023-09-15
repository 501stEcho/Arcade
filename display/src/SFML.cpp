/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** NCurses.hpp
*/

#include "SFML.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

render::SFMLDisplay::SFMLDisplay()
{
    if (!_font.loadFromFile("./assets/fonts/VeraMono.ttf"))
        throw AssetException("Cannot load font at ./assets/fonts/VeraMono.ttf");
    _window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
}

render::SFMLDisplay::~SFMLDisplay()
{
    _window.close();
}

void render::SFMLDisplay::_loadSpritesheet(
const arcade::Spritesheet &spritesheet)
{
    std::string path = spritesheet.graphicPath;
    sf::Texture texture;

    if (!texture.loadFromFile(path))
        throw AssetException("Cannot load spritesheet from " + path);
    _spritesheets[spritesheet.id] = {spritesheet.unitDims, texture};
}

void render::SFMLDisplay::loadSpritesheets(
const std::vector<arcade::Spritesheet> &spritesheets)
{
    _spritesheets.clear();
    for (const auto &spritesheet : spritesheets)
        _loadSpritesheet(spritesheet);
}

void render::SFMLDisplay::clearWin()
{
    _window.clear();
}

void render::SFMLDisplay::drawSprite(
const arcade::SpriteCostume &sprite, double x, double y)
{
    LoadedSpritesheet &spritesheet = _spritesheets[sprite.spritesheet];
    sf::Sprite spriteToRender;
    const Position &idx = sprite.pos;
    const Position &dims = spritesheet.dims;

    spriteToRender.setTexture(spritesheet.texture);
    spriteToRender.setTextureRect(
    sf::IntRect(idx.x * dims.x, idx.y * dims.y, dims.x, dims.y));
    spriteToRender.setOrigin(0, 0);
    spriteToRender.setPosition(x * SPRITE_SIZE, y * SPRITE_SIZE);
    spriteToRender.setScale(
    SPRITE_SIZE / (float)dims.x, SPRITE_SIZE / (float)dims.y);
    _window.draw(spriteToRender);
}

void render::SFMLDisplay::drawText(
const arcade::TextDisplay &text, double x, double y)
{
    sf::Text textToRender;
    textToRender.setFont(_font);
    textToRender.setString(text.content);
    textToRender.setCharacterSize(23);
    textToRender.setFillColor(sf::Color::White);
    textToRender.setPosition(x * SPRITE_SIZE, y * SPRITE_SIZE);
    _window.draw(textToRender);
}

void render::SFMLDisplay::refreshWin()
{
    _window.display();
}

std::vector<arcade::Event> render::SFMLDisplay::pollEvents()
{
    std::vector<arcade::Event> events;
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::KeyPressed) {
            if (KEYS_TRANSLATE.find(event.key.code) != KEYS_TRANSLATE.end()) {
                arcade::RawKey keyId = KEYS_TRANSLATE.at(event.key.code);
                events.push_back({keyId, {0, 0}});
            }
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f position =
            static_cast<sf::Vector2f>(sf::Mouse::getPosition(_window));
            events.push_back(
            {event.mouseButton.button == sf::Mouse::Left ? arcade::MOUSE_LEFT
                                                         : arcade::MOUSE_RIGHT,
            {position.x / SPRITE_SIZE, position.y / SPRITE_SIZE}});
        }
    }
    return events;
}

extern "C" {
render::IRenderer *entryPoint()
{
    return new render::SFMLDisplay;
}
int getType()
{
    return arcade::GRAPHICAL;
}
std::string getName()
{
    return "SFML";
}
}
