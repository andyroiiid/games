//
// Created by Andrew Huang on 7/10/2022.
//

#ifndef GAMES_NON_COPYABLE_H
#define GAMES_NON_COPYABLE_H

struct NonCopyable {
    NonCopyable() = default;

    NonCopyable(const NonCopyable &) = delete;

    NonCopyable &operator=(const NonCopyable &) = delete;
};

#endif //GAMES_NON_COPYABLE_H
