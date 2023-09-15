#include "RenderContext.hpp"
#include <iostream>

arcade::RenderContext::RenderContext()
{
}

arcade::RenderContext::~RenderContext()
{
    _renderer.reset();
}

void arcade::RenderContext::clearWin()
{
    _renderer->clearWin();
}

void arcade::RenderContext::refreshWin()
{
    _renderer->refreshWin();
}

void arcade::RenderContext::setGameName(std::string gameName)
{
    _gameName = gameName;
}

void arcade::RenderContext::setUserName(std::string userName)
{
    _userName = userName;
}

void arcade::RenderContext::drawSprite(
const SpriteCostume &sprite, double x, double y)
{
    _renderer->drawSprite(sprite, x, y);
}

void arcade::RenderContext::drawText(
const TextDisplay &text, double x, double y)
{
    _renderer->drawText(text, x, y);
}

void arcade::RenderContext::setRenderer(
std::shared_ptr<render::IRenderer> &renderer)
{
    _renderer = nullptr;
    _renderer = renderer;
}

void arcade::RenderContext::saveScore(const std::size_t &score)
{
    ScoreManager scores;
    scores.addInLeaderBoard(_gameName, {_userName, std::to_string(score)});
}
