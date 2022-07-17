//
// Created by Andrew Huang on 7/9/2022.
//

#include "proto2d.h"

#include "state/terminal.h"

std::unique_ptr<Game> NewGame() {
    return std::make_unique<Proto2D>();
}

Proto2D::Proto2D() {
    m_state = std::make_unique<Terminal>();
}

Proto2D::~Proto2D() = default;

void Proto2D::OnResize(const IntVec2 &size) {
    m_size = size;
    m_state->OnResize(size);
}

void Proto2D::OnTick(float deltaTime) {
    Update(deltaTime);
    Draw();
}

void Proto2D::OnClose() {
    RequestQuit();
}

void Proto2D::Update(float deltaTime) {
    StateBuilder builder = m_state->Update(deltaTime);
    if (builder) {
        m_state.reset();
        m_state = builder();
        m_state->OnResize(m_size);
    }
}

void Proto2D::Draw() {
    m_state->Draw();
}
