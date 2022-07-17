//
// Created by Andrew Huang on 7/9/2022.
//

#ifndef GAMES_PROTO2D_H
#define GAMES_PROTO2D_H

#include <game.h>

class Proto2D : public Game {
public:
    Proto2D();

    ~Proto2D() override;

    void OnResize(const IntVec2 &size) override;

    void OnTick(float deltaTime) override;

    void OnClose() override;

    struct State;

    typedef std::unique_ptr<State> (*StateBuilder)();

    struct State : NonCopyable {
        virtual ~State() = default;

        virtual void OnResize(const IntVec2 &size) {}

        virtual StateBuilder Update(float deltaTime) { return nullptr; }

        virtual void Draw() {}
    };

private:
    void Update(float deltaTime);

    void Draw();

    IntVec2                m_size;
    std::unique_ptr<State> m_state;
};

#endif //GAMES_PROTO2D_H
