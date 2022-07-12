//
// Created by Andrew Huang on 7/12/2022.
//

#ifndef GAMES_SINGLETON_H
#define GAMES_SINGLETON_H

#include "non_copyable.h"

template<class T>
class Singleton : NonCopyable {
public:
    static T &Get() {
        static T instance;
        return instance;
    }
};

#endif //GAMES_SINGLETON_H
