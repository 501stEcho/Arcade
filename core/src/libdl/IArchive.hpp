/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** IArchive
*/

#pragma once

class IArchive {
    public:
        ~IArchive() = default;
        class ArchError : std::exception {
            public:
            ArchError(std::string msg) : _msg(msg){};
            const char *what() const noexcept
            {
                return _msg.data();
            }

            private:
            std::string _msg;
        };
    protected:
    private:
};
