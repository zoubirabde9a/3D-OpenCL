#include "TextureCache.h"
#include "ImageLoader.h"
namespace ZEngine {

   /* TextureCache::TextureCache()
    {

    }

    TextureCache::~TextureCache()
    {
        _textureMap.clear();
    }*/

  /*  const GLTexture* TextureCache::getTexture(std::string texturePath, bool softFiltering) {

        //lookup the texture and see if its in the map
        auto mit = _textureMap.find(texturePath);

        //check if its not in the map
        if (mit == _textureMap.end()) {
            //Load the texture
            GLTexture newTexture = {};
            newTexture = ImageLoader::loadPNG(texturePath, softFiltering);

            //Insert it into the map
            return  &(_textureMap.insert(make_pair(texturePath, newTexture)).first->second);
        }
        return &(mit->second);
    }
    const GLTexture* TextureCache::getTexture() {

        //lookup the texture and see if its in the map
        auto mit = _textureMap.find("");

        //check if its not in the map
        if (mit == _textureMap.end()) {
            //Load the texture
            GLTexture newTexture = {};

            //Insert it into the map
            return  &(_textureMap.insert(std::make_pair("", newTexture)).first->second);
        }
        return &(mit->second);
    }
     const TileSheet* TextureCache::getTileSheet(std::string texturePath, const glm::ivec2 &dims, bool softFiltering) {

        //lookup the texture and see if its in the map
        auto mit = m_tileSheetMap.find(texturePath);

        //check if its not in the map
        if (mit == m_tileSheetMap.end()) {
            //Load the texture
            TileSheet newTileSheet;
            newTileSheet.init(texturePath, dims, softFiltering);

            //Insert it into the map
            return &m_tileSheetMap.insert(make_pair(texturePath, newTileSheet)).first->second;
        }
        return &(mit->second);
    }
     const TileSheet* TextureCache::getTileSheet(const Json::Value &root) {
         using namespace Json;
         const Value& pathValue = root["path"];
         std::string path;
         if (pathValue.isString()) {
             path = pathValue.asString();
         }
         else return nullptr;

         auto mit = m_tileSheetMap.find(path);
         if (mit != m_tileSheetMap.end())return &mit->second;

         const Value &dimsValue = root["dimentions"];
         glm::vec2 dims;
         if (dimsValue.isArray() && dimsValue.size() >= 2) {
             dims.x = dimsValue[0].asFloat();
             dims.y = dimsValue[1].asFloat();
         }
         else return nullptr;

         TileSheet newTileSheet(path, dims);
         return &m_tileSheetMap.insert(std::make_pair(path, newTileSheet)).first->second;
     }*/
}
}
