#include "Texture.h"

#include "ImageImporter.h"

SPtr<Texture> Texture::Load(const char* path)
{
    return ImageImporter::Import(path);
}

Texture::Texture(void* pPixels, int width, int height, int channel) :
    mWidth(width),
    mHeight(height)
{
    bool vflip = false;

    int widthSize = width * channel;
    int widthSizeAligned = (widthSize + 3) & (~3);

    mpData = malloc(sizeof(unsigned char)*widthSizeAligned * height);
    for(int i = 0; i < height; i++) {
        memcpy((unsigned char*)mpData + (vflip ? (height - 1 - i) : i) * widthSizeAligned, (unsigned char*)pPixels + i * widthSize, widthSize);
    }

    glGenTextures(1, &mTextureObject);

    glBindTexture(GL_TEXTURE_2D, mTextureObject);
    if(channel == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mpData);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mpData);

    int mip_levels = 0; for(int k = max(width, height); k; k >>= 1) mip_levels++;
    for(int l = 1; l < mip_levels; l++) {
        if(channel == 3)
            glTexImage2D(GL_TEXTURE_2D, l, GL_RGB8, max(1, width >> l), max(1, height >> l), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        else
            glTexImage2D(GL_TEXTURE_2D, l, GL_RGBA, max(1, width >> l), max(1, height >> l), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

Texture::~Texture()
{
    glDeleteTextures(1, &mTextureObject);
    free(mpData);
}
