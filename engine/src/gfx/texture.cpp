//
// Created by Andrew Huang on 7/11/2022.
//

#include "gfx/texture.h"

Texture::Texture(int width, int height, const unsigned *data, Wrap wrap, bool filter, bool mipmaps) {
    m_width  = width;
    m_height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
    glTextureStorage2D(m_texture, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(m_texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (filter) {
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    switch (wrap) {
    case Wrap::Repeat:
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    case Wrap::Clamp:
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;
    case Wrap::MirrorRepeat:
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        break;
    case Wrap::MirrorClamp:
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);
        break;
    }

    if (mipmaps) {
        glGenerateTextureMipmap(m_texture);
    }
}

Texture::~Texture() {
    if (m_texture) glDeleteTextures(1, &m_texture);
}

void Texture::Bind(GLuint unit) {
    glBindTextureUnit(unit, m_texture);
}
