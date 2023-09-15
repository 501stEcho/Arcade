/*
** EPITECH PROJECT, 2022
** arcade
** File description:
** AssetException.hpp
*/

#include <exception>
#include <string>

namespace render
{

    class AssetException : public std::exception
    {
    public:
        AssetException(std::string msg) : _msg(msg){};
        const char *what() const noexcept
        {
            return _msg.data();
        }

    private:
        std::string _msg;
    };

}
