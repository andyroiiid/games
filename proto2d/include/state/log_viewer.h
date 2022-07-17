//
// Created by andyroiiid on 7/16/2022.
//

#ifndef GAMES_LOG_VIEWER_H
#define GAMES_LOG_VIEWER_H

#include "proto2d.h"

#include <ui/text_renderer.h>

class LogViewer : public Proto2D::State {
public:
    LogViewer();

    ~LogViewer() override;

    void OnResize(const IntVec2 &size) override;

    Proto2D::StateBuilder Update(float deltaTime) override;

    void Draw() override;

private:
    float        m_fps = 0.0f;
    Vec2         m_size;
    TextRenderer m_textRenderer;
};

#endif //GAMES_LOG_VIEWER_H
