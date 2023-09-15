/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** RenderContext.hpp
*/
#include "../../../interface/IContext.hpp"
#include "../score/Score.hpp"
#include <cstddef>
#include <memory>
#include <vector>
namespace arcade
{
    class RenderContext : public IContext
    {
    public:
        RenderContext();
        ~RenderContext();
        void clearWin() override;
        void refreshWin() override;
        void drawSprite(
        const SpriteCostume &sprite, double x, double y) override;
        void drawText(
        const TextDisplay &text, double x, double y) override;
        void setRenderer(
        std::shared_ptr<render::IRenderer> &renderer);
        void setGameName(std::string gameName);
        void setUserName(std::string userName);
        void saveScore(const std::size_t &score) override;

    private:
        std::shared_ptr<render::IRenderer> _renderer = nullptr;
        std::string _gameName;
        std::string _userName;
    };
}