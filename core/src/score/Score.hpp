/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** Score
*/

#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>
#include "../ScoreData.hpp"
class ScoreManager {
    public:
        ScoreManager();
        ~ScoreManager() = default;
        void addInLeaderBoard(std::string gameName, scoreData elem);
        std::vector<scoreData> getLeaderBoard(std::string gameName);
        std::vector<std::string> split(std::string s, std::string delimiter);
    protected:
        std::vector<scoreData> _scores;
    private:
};
