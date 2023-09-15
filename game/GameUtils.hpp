/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** GameUtils.hpp
*/

#include <fstream>
#include <string>
#include <vector>

namespace game
{
    template <typename T>
    std::vector<T> loadMap(const std::string path,
    void (*cb)(double, double, char, std::vector<T> &vec))
    {
        std::size_t x = 0;
        std::size_t y = 0;

        std::ifstream map(path);
        std::string line;

        std::vector<T> ret;

        if (!map.is_open())
            return ret;

        while (std::getline(map, line)) {
            for (auto &c : line) {
                cb(x, y, c, ret);
                x++;
            }
            x = 0;
            y++;
        }
        return ret;
    }
}
