//
// Created by andyroiiid on 7/16/2022.
//

#ifndef GAMES_GRID_LAYOUT_H
#define GAMES_GRID_LAYOUT_H

#include "math/vector.h"

class GridLayout {
public:
    GridLayout(const IntVec2 &size, const IntVec2 &grids)
            : m_grid(grids),
              m_ratioPerGrid(1.0f / Vec2(grids)),
              m_gridSize(Vec2(size) * m_ratioPerGrid) {}

    [[nodiscard]] const Vec2 &GetGridSize() const { return m_gridSize; }

    [[nodiscard]] Vec4 GetGridRect(int index) const {
        const div_t result = div(index, m_grid.x);
        const Vec2  textureGrid{
                static_cast<float>(result.rem),
                static_cast<float>(result.quot)
        };
        return {textureGrid * m_ratioPerGrid, m_ratioPerGrid};
    }

private:
    IntVec2 m_grid;
    Vec2    m_ratioPerGrid;
    Vec2    m_gridSize;
};

#endif //GAMES_GRID_LAYOUT_H
