#pragma once

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "BaseHeader.h"

class Texture
{
public:
    static SPtr<Texture> Load(const char* path);

public:
    Texture(void* pPixels, int width, int height, int channel);
    ~Texture();

    GLuint GetTextureObject() const { return mTextureObject; }

private:
    int mWidth;
    int mHeight;
    int mSize;
    void* mpData;

    GLuint mTextureObject;
};

#endif // __TEXTURE_H__
