/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** Menu
*/

#include "Menu.hpp"
#include <chrono>

game::Menu::Menu(std::vector<libElem> renders, std::vector<libElem> games,
std::map<arcade::RawKey, arcade::Keymaps> keyMap)
    : _keyMap(keyMap)
{
    _start = {0, _getEmptyLayer(10), "  PRESS ENTER TO START"};
    _name = {0, _getEmptyLayer(14), "        Xx_4RK4AD3_xX"};
    _userNameHelp = {0, _getBottomEmptyLayer(15), "PRESS U FOR USERNAME MODE"};
    _keymapHelp = {0, _getBottomEmptyLayer(15), "PRESS I FOR KeyMapping MODE"};
    _type = {0, _getBottomEmptyLayer(15), "SELECT GAME AND GRAPHICAL LIBRARY"};

    _games = games;
    _renders = renders;
    _GameButton.push_back({1, _getEmptySelectedLayer(8), _games[0].name});
    _LibButton.push_back({1, _getEmptySelectedLayer(8), _renders[0].name});

    for (int i = 1; i <= 3; i++)
        _UserName.push_back({0, _getEmptyLayer(1), "A"});
    for (int i = 1; i < _games.size(); i++)
        _GameButton.push_back({0, _getEmptyLayer(8), _games[i].name});
    for (int i = 1; i < _renders.size(); i++)
        _LibButton.push_back({0, _getEmptyLayer(8), _renders[i].name});
    initSelectedMapping();
    _selectedVector = 0;
}

void game::Menu::initSelectedMapping()
{
    std::vector<arcade::Keymaps> refelem{
    arcade::Keymaps::ACTIONA,
    arcade::Keymaps::ACTIONB,
    arcade::Keymaps::LEFT,
    arcade::Keymaps::RIGHT,
    arcade::Keymaps::UP,
    arcade::Keymaps::DOWN,
    arcade::Keymaps::RESTART,
    };
    for (auto &it : _keyMap) {
        if (std::find(refelem.begin(), refelem.end(), it.second) !=
        refelem.end())
            _Keymaps.push_back({0, _getEmptyLayer(10),
            RawKeyToString(it.second) + "=" + std::to_string(it.first)});
    }
    _Keymaps[0].selected = true;
}

std::vector<game::Button> game::Menu::_getBottomEmptyLayer(int lenght)
{
    std::vector<Button> vec;
    double i = 0;
    vec.push_back({{0, 2}, HOLDER_BL_US});

    while (i < lenght) {
        vec.push_back({{1 + i, 2}, HOLDER_U_US});
        i++;
    }

    vec.push_back({{i + 1, 2}, HOLDER_BR_US});
    i = 0;

    return vec;
}

std::vector<game::Button> game::Menu::_getEmptyLayer(int lenght)
{
    std::vector<Button> vec;
    double i = 0;
    vec.push_back({{0, 0}, HOLDER_UL_US});
    while (i < lenght) {
        vec.push_back({{1 + i, 0}, HOLDER_U_US});
        i++;
    }
    vec.push_back({{i + 1, 0}, HOLDER_UR_US});
    vec.push_back({{0, 2}, HOLDER_BL_US});

    i = 0;
    while (i < lenght) {
        vec.push_back({{1 + i, 2}, HOLDER_U_US});
        i++;
    }

    vec.push_back({{i + 1, 2}, HOLDER_BR_US});
    i = 0;

    return vec;
}

std::vector<game::Button> game::Menu::_getEmptySelectedLayer(int lenght)
{
    std::vector<Button> vec;
    double i = 0;
    vec.push_back({{0, 0}, HOLDER_UL_S});
    while (i < lenght) {
        vec.push_back({{1 + i, 0}, HOLDER_U_S});
        i++;
    }
    vec.push_back({{i + 1, 0}, HOLDER_UR_S});
    vec.push_back({{0, 2}, HOLDER_BL_S});

    i = 0;
    while (i < lenght) {
        vec.push_back({{1.0 + i, 2.0}, HOLDER_U_S});
        i++;
    }

    vec.push_back({{i + 1, 2}, HOLDER_BR_S});
    i = 0;

    return vec;
}

const std::vector<arcade::Spritesheet> &game::Menu::getSpritesheets() const
{
    return SPRITESHEETS;
}

void game::Menu::compute(
const std::chrono::duration<double> &deltaTime, arcade::IContext &context)
{
    _timeReminder += deltaTime;
}

void game::Menu::render(arcade::IContext &context)
{
    displayAllButton(context);
}

std::string game::Menu::getUserName()
{
    return _UserName[0].name + _UserName[1].name + _UserName[2].name;
}

std::string game::Menu::RawKeyToString(int value)
{
    return std::to_string(value);
}

std::vector<std::string> game::Menu::split(
std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::string game::Menu::StringToKeymap(std::string value)
{
    std::vector<std::string> splited = split(value, "=");
    std::string name = _mapedKey[atoi(splited[0].c_str())];
    std::string keyBind = _rawKey[atoi(splited[1].c_str())];
    return name + " = " + keyBind;
}

std::map<arcade::RawKey, arcade::Keymaps> game::Menu::getUpdatedKeyMap()
{
    for (int i = 0; i < _Keymaps.size(); i++){
        std::vector<std::string> splited = split(_Keymaps[i].name, "=");
        int name = atoi(splited[0].c_str());
        int keyBind = atoi(splited[1].c_str());
        _keyMap[static_cast<arcade::RawKey>(keyBind)] = static_cast<arcade::Keymaps>(name);
    }

    return _keyMap;
}

void game::Menu::updateKey(arcade::RawKey input)
{
    int i = 0;
    for (i = 0; i < _Keymaps.size() && !_Keymaps[i].selected; i++)
        ;
    std::string value = _Keymaps[i].name;
    std::vector<std::string> splited = split(value, "=");
    std::string name = splited[0];
    std::string keyBind = _rawKey[atoi(splited[1].c_str())];
    int rawkeyBind = atoi(splited[1].c_str());
    _keyMap[static_cast<arcade::RawKey>(rawkeyBind)] = arcade::NONE;
    _Keymaps[i].name = name + "="+std::to_string(input);

}

void game::Menu::handleInput(const arcade::KeyInput &key)
{
    if (_recording) {
        updateKey(key.keyEvent.key);
        _recording = false;
        return;
    }
    if (_state == game::Menu::MenuState::MENU &&
    (key.mapping == arcade::UP || key.mapping == arcade::DOWN))
        _selectedColumn = !_selectedColumn;
    if (_state == game::Menu::MenuState::USERNAME &&
    (key.mapping == arcade::LEFT || key.mapping == arcade::RIGHT))
        _selectedVector += key.mapping == arcade::LEFT ? -1 : 1;
    if ((_state == game::Menu::MenuState::KEYMAPPING) &&
    key.keyEvent.key == arcade::K_SPACE)
        _recording = true;
    updateMenuState(key);
    std::vector<FullButton> vector = getSelectedVector();

    resetStates(vector);
    HandleVecSelection(vector, key);

    if (_state == game::Menu::MenuState::USERNAME) {
        _UserName = vector;
        return;
    }
    if (_state == game::Menu::MenuState::MENU)
        if (_selectedColumn)
            _GameButton = vector;
        else
            _LibButton = vector;
    if (_state == game::Menu::MenuState::KEYMAPPING)
        _Keymaps = vector;
}

int game::Menu::getSelectedGameIdx()
{
    int i = 0;
    for (auto &elem : _GameButton) {
        if (elem.selected)
            return i;
        i++;
    }
    return i;
}

std::string game::Menu::getGameName()
{
    int i = 0;
    for (auto &elem : _GameButton) {
        if (elem.selected)
            return elem.name;
        i++;
    }
    return "GAME";
}

int game::Menu::getSelectedRenderIdx()
{
    int i = 0;
    for (auto &elem : _LibButton) {
        if (elem.selected)
            return i;
        i++;
    }
    return i;
}

void game::Menu::updateMenuState(const arcade::KeyInput &key)
{
    if (_state != game::Menu::MenuState::USERNAME &&
    key.mapping == arcade::SUBMENU1)
        _state = game::Menu::MenuState::USERNAME;
    else if (_state != game::Menu::MenuState::KEYMAPPING &&
    key.mapping == arcade::SUBMENU2)
        _state = game::Menu::MenuState::KEYMAPPING;
    else if (_state != game::Menu::MenuState::MENU &&
    (key.mapping == arcade::SUBMENU1 || key.mapping == arcade::SUBMENU2))
        _state = game::Menu::MenuState::MENU;
}

void game::Menu::_displayButton(
arcade::IContext &context, const FullButton button, int x, int y)
{
    for (int i = 0; i < button.ButtonVector.size(); i++) {
        context.drawSprite(COSTUMES.at(button.ButtonVector[i].costume),
        button.ButtonVector[i].pos.x + x, button.ButtonVector[i].pos.y + y);
    }
}

bool game::Menu::_shouldMove()
{
    if (_timeReminder >= TIME_TO_MOVE) {
        _timeReminder -= TIME_TO_MOVE;
        _isHide = HIDLE_TIME;
        return true;
    }
    return false;
}

void game::Menu::displayMenu(arcade::IContext &context)
{
    int i = 0;

    for (i = 0; i < _GameButton.size() && !_GameButton[i].selected; i++)
        ;
    game::FullButton selectedGame = _GameButton[i];
    int selectedGameIdx = i + 1;
    for (i = 0; i < _LibButton.size() && !_LibButton[i].selected; i++)
        ;
    game::FullButton selectedRender = _LibButton[i];
    int selectedRenderIdx = i + 1;

    context.drawText({"selected Game: " + std::to_string(selectedGameIdx) +
                     "/" + std::to_string(_GameButton.size()), 7, 0},
    3, 9);
    context.drawText({"<--", 7, 0}, 2, 11);
    context.drawText({selectedGame.name, 7, 0}, 5, 11);
    context.drawText({"-->", 7, 0}, 10, 11);

    if (_selectedColumn && selectedGame.selected &&
    _isHide < std::chrono::duration<double>::zero())
        _displayButton(context, selectedGame, 2, 10);
    else if (!_selectedColumn)
        _displayButton(context, selectedGame, 2, 10);

    context.drawText({"selected Render: " + std::to_string(selectedRenderIdx) +
                     "/" + std::to_string(_LibButton.size()), 7, 0},
    3, 14);
    context.drawText({"<--", 7, 0}, 2, 16);
    context.drawText({selectedRender.name, 7, 0}, 5, 16);
    context.drawText({"-->", 7, 0}, 10, 16);
    if (!_selectedColumn && selectedRender.selected &&
    _isHide < std::chrono::duration<double>::zero())
        _displayButton(context, selectedRender, 2, 15);
    else if (_selectedColumn)
        _displayButton(context, selectedRender, 2, 15);
}

void game::Menu::displayScore(arcade::IContext &context)
{
    context.drawText({"SCOREBOARD:", 7, 0}, 19, 15);
    int i = 0;
    for (i; i < _GameButton.size() && !_GameButton[i].selected; i++)
        ;
    std::vector<scoreData> scores =
    _scoreManager.getLeaderBoard(_GameButton[i].name);
    for (i = 0; (i < scores.size()) && (i < 5); i++) {
        context.drawText({scores[i].userName, 7, 0}, 19, 17 + i);
        context.drawText({"=", 7, 0}, 21, 17 + i);
        context.drawText({scores[i].score, 7, 0}, 22, 17 + i);
    }
}

void game::Menu::displayUsername(arcade::IContext &context)
{
    int y = 0;

    for (int i = 0; i < _UserName.size(); i++) {
        context.drawText({_UserName[i].name, 7, 0}, (y * 6  + 9) / 2, 10);
        y += 1;
        if ((y - 1 == _selectedVector) &&
        _isHide > std::chrono::duration<double>::zero())
            continue;
        _displayButton(context, _UserName[i], (y * 3), 9);
    }
}

void game::Menu::displayKeyMapping(arcade::IContext &context)
{
    int i = 0;

    for (i = 0; i < _Keymaps.size() && !_Keymaps[i].selected; i++)
        ;
    game::FullButton selectedKey = _Keymaps[i];
    int selectedKeyIdx = i + 1;

    context.drawText({"selected Key: " + std::to_string(selectedKeyIdx) + "/" +
                     std::to_string(_Keymaps.size()), 7, 0},
    4, 9);
    context.drawText({"<--", 7, 0}, 0, 11);
    context.drawText({StringToKeymap(selectedKey.name), 7, 0}, 4, 11);
    context.drawText({"-->", 7, 0}, 14, 11);
    
    
    context.drawText({"PRESS SPACE TO START RECORDING", 7, 0}, 0, 13);

    if (_recording && _isHide < std::chrono::duration<double>::zero())
        _displayButton(context, selectedKey, 2, 10);
    else if (!_recording)
        _displayButton(context, selectedKey, 2, 10);

}

void game::Menu::displaySelectedWindow(arcade::IContext &context)
{
    if (_state == game::Menu::MenuState::USERNAME) {
        context.drawText({"SELECT USERNAME", 7, 0}, 0, 6);
        displayUsername(context);
    }
    if (_state == game::Menu::MenuState::KEYMAPPING) {
        context.drawText({"SET UP KEYMAPPING", 7, 0}, 0, 6);
        displayKeyMapping(context);
    }
    if (_state == game::Menu::MenuState::MENU) {
        context.drawText({_type.name, 7, 0}, 0, 6);
        displayMenu(context);
    }
}

void game::Menu::displayAllButton(arcade::IContext &context)
{
    context.drawText({_name.name, 7, 0}, 0, 1);
    _displayButton(context, _name, 0, 0);

    _tick = _tick > 100 ? 1 : _tick + 1;
    int y = 7;
    _shouldMove();

    _displayButton(context, _type, 0, 5);
    displaySelectedWindow(context);

    _displayButton(context, _keymapHelp, 20, 8);
    context.drawText({_keymapHelp.name, 7, 0}, 20, 9);

    _displayButton(context, _userNameHelp, 20, 11);
    context.drawText({_userNameHelp.name, 7, 0}, 20, 12);

    displayScore(context);
    context.drawText({_start.name, 7, 0}, 1, y * 3 + 1);
    _displayButton(context, _start, 1, y * 3);
    _isHide--;
}

void game::Menu::apply_size(int value, std::vector<Button> &buttons)
{
    for (auto &button : buttons) {
        if (value > 0)
            button.costume = static_cast<game::Costume>(
            button.costume >= 6 ? button.costume : button.costume + value);
        if (value < 0)
            button.costume = static_cast<game::Costume>(
            button.costume < 6 ? button.costume : button.costume + value);
    }
}

void game::Menu::resetStates(std::vector<FullButton> &vector)
{
    if (_state == game::Menu::MenuState::MENU ||
    _state == game::Menu::MenuState::KEYMAPPING)
        for (_selectedVector = 0; _selectedVector < vector.size() &&
             !vector[_selectedVector].selected;
             _selectedVector++)
            ;
    for (auto &button : vector) {
        button.selected = false;
        apply_size(6, button.ButtonVector);
    }
};

std::vector<game::FullButton> &game::Menu::getSelectedVector()
{
    if (_state == game::Menu::MenuState::USERNAME)
        return _UserName;
    if (_state == game::Menu::MenuState::MENU)
        if (_selectedColumn)
            return _GameButton;
        else
            return _LibButton;
    if (_state == game::Menu::MenuState::KEYMAPPING)
        return _Keymaps;
    return _GameButton;
}

void game::Menu::HandleMenuVecSelection(
std::vector<FullButton> &vector, const arcade::KeyInput &key)
{
    if (key.mapping == arcade::LEFT)
        _selectedVector--;
    if (key.mapping == arcade::RIGHT)
        _selectedVector++;
    if (_selectedVector < 0)
        _selectedVector = vector.size() - 1;
    if (_selectedVector >= vector.size())
        _selectedVector = 0;
    apply_size(-6, vector[_selectedVector].ButtonVector);
}

void game::Menu::HandleUsernameVecSelection(
std::vector<FullButton> &vector, const arcade::KeyInput &key)
{
    if (_selectedVector < 0)
        _selectedVector = vector.size() - 1;
    if (_selectedVector >= vector.size())
        _selectedVector = 0;
    if (key.mapping == arcade::UP)
        vector[_selectedVector].name[0] += 1;
    if (key.mapping == arcade::DOWN)
        vector[_selectedVector].name[0] -= 1;
    if (vector[_selectedVector].name[0] > 'Z' ||
    vector[_selectedVector].name[0] < 'A')
        vector[_selectedVector].name[0] =
        vector[_selectedVector].name[0] > 'Z' ? 'A' : 'Z';
}

void game::Menu::HandleKeyMapVecSelection(
std::vector<FullButton> &vector, const arcade::KeyInput &key)
{
    if (key.mapping == arcade::LEFT)
        _selectedVector--;
    if (key.mapping == arcade::RIGHT)
        _selectedVector++;
    if (_selectedVector < 0)
        _selectedVector = vector.size() - 1;
    if (_selectedVector >= vector.size())
        _selectedVector = 0;
    apply_size(-6, vector[_selectedVector].ButtonVector);
}

void game::Menu::HandleVecSelection(
std::vector<FullButton> &vector, const arcade::KeyInput &key)
{
    if (_state == game::Menu::MenuState::MENU)
        HandleMenuVecSelection(vector, key);
    if (_state == game::Menu::MenuState::USERNAME)
        HandleUsernameVecSelection(vector, key);
    if (_state == game::Menu::MenuState::KEYMAPPING)
        HandleKeyMapVecSelection(vector, key);
    vector[_selectedVector].selected = true;
}