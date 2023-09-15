/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** main
*/
#include "../Arcade.hpp"
#include "Core.hpp"
#include <chrono>
#include "score/Score.hpp"

int main(int argc, char **argv)
{
    Core core;

    if (argc != 2) {
        std::cerr << "USAGE: ./arcade {graphical lib path}" << std::endl;
        return 84;
    }
    try {
        core.init(argv[1]);
    } catch (Core::CoreError err) {
        std::cerr << err.what() << std::endl;
        return 84;
    }

    core.loadGameState();
    auto last = std::chrono::high_resolution_clock::now();
    while (1) {
        auto now = std::chrono::high_resolution_clock::now();
        auto diff = now - last;
        if(!core.updateGameState(diff))
            break;
        last = now;
    }
}