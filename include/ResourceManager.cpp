#include "ResourceManager.h"
#include "TileSheet.h"
#include "ImageLoader.h"
namespace ZEngine {

    TextureCache ResourceManager::_textureCache;

/*    const GLTexture* ResourceManager::getTexture(const std::string &texturePath, bool softFiletring) {
        return _textureCache.getTexture(texturePath, softFiletring);
    }
*/
   /* const TileSheet* ResourceManager::getTileSheet(const std::string &texturePath, const glm::ivec2 &dims, bool softFiletring) {
        return _textureCache.getTileSheet(texturePath, dims, softFiletring);
    }
    const TileSheet* ResourceManager::getTileSheet(const Json::Value &root) {
        return _textureCache.getTileSheet(root);
    }*/
}
}
