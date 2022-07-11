//
// Created by Andrew Huang on 7/11/2022.
//

#ifndef GAMES_TEXTURE_H
#define GAMES_TEXTURE_H

#include <glad/gl.h>

#include "movable.h"

class Texture {
public:
    enum class Wrap {
        Repeat,
        Clamp,
        MirrorRepeat,
        MirrorClamp
    };

    Texture() = default;

    Texture(Texture &&) noexcept = default;

    Texture &operator=(Texture &&) noexcept = default;

    Texture(
            int width,
            int height,
            const unsigned *data, // rgba32
            Wrap wrap = Wrap::Repeat,
            bool filter = false,
            bool mipmaps = false
    );

    ~Texture();

    [[nodiscard]] inline int Width() const { return m_width; }

    [[nodiscard]] inline int Height() const { return m_height; }

    void Bind(GLuint unit);

private:
    Movable<int>    m_width;
    Movable<int>    m_height;
    Movable<GLuint> m_texture;
};

#endif //GAMES_TEXTURE_H
