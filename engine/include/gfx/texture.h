//
// Created by Andrew Huang on 7/11/2022.
//

#ifndef GAMES_TEXTURE_H
#define GAMES_TEXTURE_H

#include <glad/gl.h>

#include "movable.h"
#include "math/vector.h"

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
            const IntVec2 &size,
            const unsigned *data,
            Wrap wrap = Wrap::Repeat,
            bool filter = false,
            bool mipmaps = false
    );

    ~Texture();

    [[nodiscard]] inline const IntVec2 &Size() const { return m_size; }

    void Bind(GLuint unit);

private:
    Movable<IntVec2> m_size;
    Movable<GLuint>  m_texture;
};

#endif //GAMES_TEXTURE_H
