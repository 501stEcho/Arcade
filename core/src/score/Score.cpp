/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** Score
*/

#include "Score.hpp"

ScoreManager::ScoreManager()
{
}

void ScoreManager::addInLeaderBoard(std::string gameName, scoreData elem)
{
    std::vector<scoreData> actualScoreList = getLeaderBoard(gameName);
    std::fstream fileData;

    int i = 0;

    for (i = 0; i < actualScoreList.size(); i++) {
        if (!strcmp(
            actualScoreList[i].userName.c_str(), elem.userName.c_str()))
            break;
    }
    if (i == actualScoreList.size())
        actualScoreList.push_back({elem.userName, elem.score});
    else {
        actualScoreList[i].userName = elem.userName;
        actualScoreList[i].score = elem.score > actualScoreList[i].score ? elem.score : actualScoreList[i].score;
    }

    fileData.open("scores/" + gameName,
    std::fstream::in | std::fstream::out | std::fstream::trunc);

    for (i = 0; i < actualScoreList.size(); i++) {
        fileData << actualScoreList[i].userName << ":"
                 << actualScoreList[i].score << "\n";
    }
    fileData.close();
}

std::vector<std::string> ScoreManager::split(
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

std::vector<scoreData> ScoreManager::getLeaderBoard(std::string gameName)
{
    std::string line;
    std::fstream fileData("scores/" + gameName);

    _scores.clear();
    if (!fileData.is_open()) {
        fileData.open("scores/" + gameName,
        std::fstream::in | std::fstream::out | std::fstream::trunc);
        fileData << "\n";
        fileData.close();
        fileData.open("scores/" + gameName);
    }
    while (fileData >> line) {
        if (line.find(":") == std::string::npos)
            continue;
        std::vector<std::string> splited = split(line, ":");
        std::string name = splited[0].length() ? splited[0] : "AAA";
        std::string score = splited[1].length() ? splited[1] : "AAA";
        for (auto &c : name)
            c = toupper(c);
        _scores.push_back({name.substr(0, 3), score.substr(0, 10)});
    }
    fileData.close();
    return _scores;
}
