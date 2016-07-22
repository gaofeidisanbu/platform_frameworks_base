/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_HWUI_TEXTURE_H
#define ANDROID_HWUI_TEXTURE_H

#include <GLES2/gl2.h>

namespace android {
namespace uirenderer {

class Caches;
class UvMapper;

/**
 * Represents an OpenGL texture.
 */
class Texture {
public:
    explicit Texture(Caches& caches) : mCaches(caches) { }

    virtual ~Texture() { }

    inline void setWrap(GLenum wrap, bool bindTexture = false, bool force = false,
                GLenum renderTarget = GL_TEXTURE_2D) {
        setWrapST(wrap, wrap, bindTexture, force, renderTarget);
    }

    virtual void setWrapST(GLenum wrapS, GLenum wrapT, bool bindTexture = false,
            bool force = false, GLenum renderTarget = GL_TEXTURE_2D);

    inline void setFilter(GLenum filter, bool bindTexture = false, bool force = false,
                GLenum renderTarget = GL_TEXTURE_2D) {
        setFilterMinMag(filter, filter, bindTexture, force, renderTarget);
    }

    virtual void setFilterMinMag(GLenum min, GLenum mag, bool bindTexture = false,
            bool force = false, GLenum renderTarget = GL_TEXTURE_2D);

    /**
     * Convenience method to call glDeleteTextures() on this texture's id.
     */
    void deleteTexture() const;

    /**
     * Name of the texture.
     */
    GLuint id = 0;
    /**
     * Generation of the backing bitmap,
     */
    uint32_t generation = 0;
    /**
     * Indicates whether the texture requires blending.
     */
    bool blend = false;
    /**
     * Width of the backing bitmap.
     */
    uint32_t width = 0;
    /**
     * Height of the backing bitmap.
     */
    uint32_t height = 0;
    /**
     * Indicates whether this texture should be cleaned up after use.
     */
    bool cleanup = false;
    /**
     * Optional, size of the original bitmap.
     */
    uint32_t bitmapSize = 0;
    /**
     * Indicates whether this texture will use trilinear filtering.
     */
    bool mipMap = false;

    /**
     * Optional, pointer to a texture coordinates mapper.
     */
    const UvMapper* uvMapper = nullptr;

    /**
     * Whether or not the Texture is marked in use and thus not evictable for
     * the current frame. This is reset at the start of a new frame.
     */
    void* isInUse = nullptr;

private:
    /**
     * Last wrap modes set on this texture.
     */
    GLenum mWrapS = GL_CLAMP_TO_EDGE;
    GLenum mWrapT = GL_CLAMP_TO_EDGE;

    /**
     * Last filters set on this texture.
     */
    GLenum mMinFilter = GL_NEAREST;
    GLenum mMagFilter = GL_NEAREST;

    bool mFirstFilter = true;
    bool mFirstWrap = true;

    Caches& mCaches;
}; // struct Texture

class AutoTexture {
public:
    explicit AutoTexture(const Texture* texture): mTexture(texture) { }
    ~AutoTexture() {
        if (mTexture && mTexture->cleanup) {
            mTexture->deleteTexture();
            delete mTexture;
        }
    }

private:
    const Texture* mTexture;
}; // class AutoTexture

}; // namespace uirenderer
}; // namespace android

#endif // ANDROID_HWUI_TEXTURE_H
