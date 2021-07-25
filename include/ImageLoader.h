#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "GLTexture.h"

#include <string>

namespace ZEngine {

    //Loads images into GLTextures
    class ImageLoader
    {
    public:
        static GLTexture loadPNG(std::string filePath, bool softFiltring = true);
        static GLTexture loadTexture(float width, float height, bool softFiltering = false);
    };

}

#endif // IMAGELOADER_H
