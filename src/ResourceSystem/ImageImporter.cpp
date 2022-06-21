#include "ImageImporter.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Texture.h"

SPtr<Texture> ImageImporter::Import(const char* path)
{
    int width, height, channel;
    stbi_uc* imageData;
    imageData = stbi_load(path, &width, &height, &channel, STBI_rgb_alpha);

    SPtr<Texture> texture = std::make_shared<Texture>(imageData, width, height, 4);

    stbi_image_free(imageData);

    return texture;
}
