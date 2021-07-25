#pragma once
//This file holds some global error functions

#include <QString>

namespace ZEngine {

    extern void fatalErrorQ(const QString &errorString);
    extern void fatalError(const std::string &errorString);

}
