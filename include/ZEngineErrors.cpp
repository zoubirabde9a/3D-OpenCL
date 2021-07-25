#include "ZEngineErrors.h"

#include <cstdlib>
#include <iostream>

namespace ZEngine {

    //Prints out an error message and exits the game
    void fatalErrorQ(const QString &errorString) {
        std::cout << errorString.toUtf8().constData() << std::endl;
        std::cout << "Enter any key to quit...";
        int tmp;
        std::cin >> tmp;
        exit(69);
    }
    void fatalError(const std::string &errorString) {
        std::cout << errorString << std::endl;
        std::cout << "Enter any key to quit...";
        int tmp;
        std::cin >> tmp;
        exit(69);
    }
}
