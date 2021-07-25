#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H


#include "TextureCache.h"
#include <string>
#include <glm\glm.hpp>
namespace ZEngine {
    //This is a way for us to access all our resources, such as
    //Models or textures.
    class ResourceManager
    {
    public:
     //   static const GLTexture* getTexture(const std::string &texturePath, bool softFiltering = true);
      /*  static const TileSheet* getTileSheet(const std::string &texturePath, const glm::ivec2 &dims = glm::ivec2(32, 32), bool softFiltering = true);
        static const TileSheet* getTileSheet(const Json::Value &root);*/
    private:
        static TextureCache _textureCache;
    };
}

#endif // RESOURCEMANAGER_H
