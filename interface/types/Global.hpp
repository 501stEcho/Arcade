#pragma once

#include <cstddef>
#include <vector>

template <typename T> using matrix = std::vector<std::vector<T>>;

struct Position {
    std::size_t x;
    std::size_t y;
};

struct Coords {
    double x;
    double y;
};

inline bool operator==(const Coords &coord1, const Coords &coord2)
{
    return (coord1.x == coord2.x && coord1.y == coord2.y);
}

inline bool operator==(const Position &coord1, const Position &coord2)
{
    return (coord1.x == coord2.x && coord1.y == coord2.y);
}

inline Coords operator+(const Coords &coord1, const Coords &coord2)
{
    Coords coo = {coord1.x + coord2.x, coord1.y + coord2.y};
    return coo;
}

template <typename T>
inline std::vector<T> operator+(const std::vector<T> &vec1, const std::vector<T> &vec2)
{
    std::vector<T> vec = vec1;
    for (auto &elem : vec2)
        vec.push_back(elem);
    return vec;
}

inline Coords &operator+=(Coords &coord1, const Coords &coord2)
{
    coord1.x += coord2.x;
    coord1.y += coord2.y;
    return coord1;
}

namespace arcade {
    enum LibType { GRAPHICAL = 1, GAME };
}