/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** Menu
*/

#pragma once

#include "../../../interface/IGame.hpp"
#include "../LibElem.hpp"
#include "../ScoreData.hpp"
#include "../score/Score.hpp"
#include <chrono>
#include <cstddef>
#include <map>
#include <string>
#include <vector>

#define TIME_TO_MOVE std::chrono::duration<double, std::ratio<1, 1>>(1)
#define HIDLE_TIME std::chrono::duration<double, std::ratio<1, 1>>(14)

namespace game
{
    enum Spritesheets { HOLDER };

    enum Costume {
        HOLDER_BL_S,
        HOLDER_B_S,
        HOLDER_BR_S,
        HOLDER_UR_S,
        HOLDER_U_S,
        HOLDER_UL_S,
        HOLDER_BL_US,
        HOLDER_B_US,
        HOLDER_BR_US,
        HOLDER_UR_US,
        HOLDER_U_US,
        HOLDER_UL_US,
    };

    const std::map<std::size_t, arcade::SpriteCostume> COSTUMES = {
    {HOLDER_UL_S, {HOLDER, {0, 0}}},
    {HOLDER_U_S, {HOLDER, {1, 0}}},
    {HOLDER_UR_S, {HOLDER, {2, 0}}},
    {HOLDER_BL_S, {HOLDER, {0, 1}}},
    {HOLDER_B_S, {HOLDER, {1, 1}}},
    {HOLDER_BR_S, {HOLDER, {2, 1}}},

    {HOLDER_UL_US, {HOLDER, {0, 2}}},
    {HOLDER_U_US, {HOLDER, {1, 2}}},
    {HOLDER_UR_US, {HOLDER, {2, 2}}},
    {HOLDER_BL_US, {HOLDER, {0, 3}}},
    {HOLDER_B_US, {HOLDER, {1, 3}}},
    {HOLDER_BR_US, {HOLDER, {2, 3}}},
    };

    const std::vector<arcade::Spritesheet> SPRITESHEETS = {
    {HOLDER, "./assets/menu/holder.jpg", "./assets/menu/holder.txt", {64, 64}}};

    typedef struct ButtonS {
        Coords pos;
        Costume costume;
    } Button;

    typedef struct FULBUTON {
        bool selected = 0;
        std::vector<Button> ButtonVector;
        std::string name;
    } FullButton;

    enum Direction {
        UP = 1,
        DOWN,
    };

    class Menu : public IGame
    {
    public:
        enum MenuState {
            MENU,
            USERNAME,
            KEYMAPPING,
        };
        Menu(std::vector<libElem> _renders, std::vector<libElem> _games,
        std::map<arcade::RawKey, arcade::Keymaps> _keyMap);
        ~Menu() = default;
        int getSelectedGameIdx();
        int getSelectedRenderIdx();
        const std::vector<arcade::Spritesheet> &
        getSpritesheets() const override;
        void compute(const std::chrono::duration<double> &dtime,
        arcade::IContext &context) override;
        void handleInput(const arcade::KeyInput &key) override;
        void render(arcade::IContext &context) override;
        std::string getUserName();
        std::string getGameName();
        std::map<arcade::RawKey, arcade::Keymaps> getUpdatedKeyMap();


    private:
        void displayAllButton(arcade::IContext &context);
        bool _shouldMove();
        void displayScore(arcade::IContext &context);
        void displayMenu(arcade::IContext &context);
        void displayUsername(arcade::IContext &context);
        void displayKeyMapping(arcade::IContext &context);
        void updateKey(arcade::RawKey input);
        void initSelectedMapping();
        std::string RawKeyToString(int value);
        std::string StringToKeymap(std::string value);
        std::vector<std::string> split(std::string s, std::string delimiter);

        void apply_size(int value, std::vector<Button> &button);
        void _displayButton(
        arcade::IContext &context, const FullButton button, int x, int y);
        void _moveSelected();
        std::vector<FullButton> &getSelectedVector();

        void HandleVecSelection(
        std::vector<FullButton> &vector, const arcade::KeyInput &key);
        void HandleMenuVecSelection(
        std::vector<FullButton> &vector, const arcade::KeyInput &key);
        void HandleUsernameVecSelection(
        std::vector<FullButton> &vector, const arcade::KeyInput &key);
        void HandleKeyMapVecSelection(
        std::vector<FullButton> &vector, const arcade::KeyInput &key);

        void resetStates(std::vector<FullButton> &vector);
        void updateMenuState(const arcade::KeyInput &key);
        void displaySelectedWindow(arcade::IContext &context);
        std::vector<game::Button> _getEmptySelectedLayer(int lenght);
        std::vector<Button> _getEmptyLayer(int lenght);
        std::vector<Button> _getBottomEmptyLayer(int lenght);

        std::vector<FullButton> _LibButton;
        std::vector<FullButton> _GameButton;
        std::vector<FullButton> _UserName;
        std::vector<FullButton> _Keymaps;

        FullButton _name;
        FullButton _userNameHelp;
        FullButton _keymapHelp;
        FullButton _type;
        FullButton _start;

        std::vector<libElem> _games;
        std::vector<libElem> _renders;

        ScoreManager _scoreManager;
        std::map<arcade::RawKey, arcade::Keymaps> _keyMap;

        MenuState _state = MenuState::MENU;
        bool _selectedColumn = true;
        int _selectedVector = 0;
        int _selectedKey = 0;
        int _tick = 0;
        bool _recording = false;
        std::vector<std::string> _mapedKey{"NONE", "UP", "DOWN", "LEFT",
        "RIGHT", "PAUSE", "PREV_GAME", "NEXT_GAME", "PREV_GRAPH", "NEXT_GRAPH",
        "RESTART", "MENU", "QUIT", "ENTER", "SUBMENU1", "SUBMENU2", "ACTIONA",
        "ACTIONB"};
        std::vector<std::string> _rawKey{"A", "B", "C", "D", "E", "F", "G",
        "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
        "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8",
        "9", "SPACE", "ESCAPE", "ENTER", "BACKSPACE", "UP", "DOWN", "LEFT",
        "RIGHT"};
        std::chrono::duration<double> _timeReminder = std::chrono::duration<double>::zero();
        std::chrono::duration<double> _isHide =
        std::chrono::duration<double>::zero();
    };
}
