/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** LibDl
*/

#pragma once
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <set>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <dlfcn.h>
#include <ctime>
#include "IArchive.hpp"
class Archive : public IArchive{
    public:
        Archive(std::string filename, int flags = RTLD_NOW);
        ~Archive() = default;
        template <typename T>
        T *getSymbol(std::string symbolName){
            T *(*res)(void) = reinterpret_cast<T *(*)()>(dlsym(_arch.get(), symbolName.c_str()));
            if (!res)
                throw ArchError(dlerror());
            return res();
        }

        template <typename T>
        T getType(std::string symbolName){
            T (*res)(void) = reinterpret_cast<T (*)()>(dlsym(_arch.get(), symbolName.c_str()));
            if (!res)
                throw ArchError(dlerror());
            return res();
        }

    private:
        std::shared_ptr<void> _arch;
};

