//
// Created by Andrew Huang on 7/9/2022.
//

#ifndef GAMES_GAME_H
#define GAMES_GAME_H

#include "math/vector.h"
#include <memory>
#include <string>

// use RAII to handle init & shutdown
struct Game {
    Game() = default;

    virtual ~Game() = default;

    virtual void OnResize(const IntVec2 &size) = 0;

    virtual void OnTick(float deltaTime) = 0;

    virtual void OnClose() = 0;
};

// post a quit request to message loop
void RequestQuit();

// allocates a new Game object
// implemented by actual game code
std::unique_ptr<Game> NewGame();

#endif //GAMES_GAME_H
