#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include <map>
#include <unordered_map>
#include "GLTexture.h"
#include <glm\glm.hpp>
#include "JsonFunctions.h"
#include "TileSheet.h"

struct lua_State;
namespace luabridge {
    class LuaRef;
}
namespace ZEngine {
    //This caches the textures so that multiple sprites can use the same textures
    class TextureCache
    {
    public:
     /*   TextureCache();
        ~TextureCache();*/

      /*  const GLTexture* getTexture(std::string texturePath, bool softFiltering = true);
        const GLTexture* getTexture();
        const TileSheet* getTileSheet(std::string texturePath, const glm::ivec2 &dims, bool softFiletring = true);
        const TileSheet* getTileSheet(const Json::Value &root);*/

    private:
        std::map < std::string, GLTexture > _textureMap;
        std::map < std::string, TileSheet > m_tileSheetMap;
    };

#endif // TEXTURECACHE_H
