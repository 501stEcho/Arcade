/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** Archive
*/

#include "Archive.hpp"

Archive::Archive(std::string filename, int flag) : _arch(dlopen(filename.c_str(), flag), [](void *_arch) -> int { return _arch ? dlclose(_arch) : 0; })
{
    if (!_arch.get()) 
        throw ArchError(dlerror());
}