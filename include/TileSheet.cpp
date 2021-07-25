#include "TileSheet.h"
#include "ResourceManager.h"


namespace ZEngine {
    glm::vec4 TileSheet::getUvs(const glm::ivec4 &Rect, const GLTexture* texture) {
        glm::vec4 uv;
        uv.x = (float)Rect.x / (float)texture->width;
        uv.y = (float)Rect.y / (float)texture->height;
        uv.z = (float)Rect.z / (float)texture->width;
        uv.w = (float)Rect.w / (float)texture->height;
        return uv;
    }
    TileSheet::TileSheet(const std::string &filePath, const glm::ivec2 &tileDims, bool softfiltering) {
        init(filePath, tileDims, softfiltering);
    }
    void TileSheet::init(const std::string &filePath) {
        m_texture = ResourceManager::getTexture(filePath);
    }
    void TileSheet::init(const std::string &filePath, const glm::ivec2 &tileDims, bool softFiltering) {
        m_texture = ResourceManager::getTexture(filePath, softFiltering);
        m_tileDims = tileDims;
    }
    void TileSheet::setDims(const glm::ivec2 &tileDims) {
        m_tileDims = tileDims;
    }
    int TileSheet::getMaxIndex() const {
        const glm::ivec2 TilesNumber(m_texture->width / m_tileDims.x, m_texture->height / m_tileDims.y);
        return TilesNumber.x * TilesNumber.y - 1;
    }
    glm::vec4 TileSheet::getUvs(int index) const {
        const glm::ivec2 TilesNumber(m_texture->width / m_tileDims.x, m_texture->height / m_tileDims.y);
        int xTile = index % TilesNumber.x;
        int yTile = floor((float)index / (float)TilesNumber.x);

        glm::vec4 uvs;
        uvs.x = (float)xTile / (float)TilesNumber.x;
        uvs.y = (float)yTile / (float)TilesNumber.y;
        uvs.z = 1.0f / (float)TilesNumber.x;
        uvs.w = 1.0f / (float)TilesNumber.y;
        return uvs;
    }
    int TileSheet::getIndex(const glm::vec4 &uvs) const {
        return uvs.x * (float)m_texture->width / m_tileDims.x ;
    }
    glm::vec4 TileSheet::getUvs(glm::ivec4 Rect) const {
        glm::vec4 uv;
        uv.x = (float)Rect.x / (float)m_texture->width;
        uv.y = (float)Rect.y / (float)m_texture->height;
        uv.z = (float)Rect.z / (float)m_texture->width;
        uv.w = (float)Rect.w / (float)m_texture->height;
        return uv;
    }
    void TileSheet::Container::addTileSheet(const TileSheet* tileSheet, const std::string &name) {
        m_tileSheets.push_back({ tileSheet, name });
    }
    TileSheet::Container::Slot TileSheet::Container::getTileSheet(const std::string &tileSheet) {
        return {};
    }
    TileSheet::Container::Tile TileSheet::Container::getSlot(GLuint index) const {
        if (index == -1)return{};
        int offset = 0;
        Tile slot;
        for (auto &it : m_tileSheets) {
            if (index < it.tileSheet->getMaxIndex() + offset) {
                slot.tileSheet = it.tileSheet;
                slot.index = index - offset;
                return slot;
            }
            else {
                offset += it.tileSheet->getMaxIndex();
            }
        }
        return slot;
    }
    GLuint TileSheet::Container::getIndex(const TileSheet* tileSheet, GLuint index) const {
        GLuint result = -1;
        int offset = 0;
        for (auto &it : m_tileSheets) {
            if (it.tileSheet == tileSheet) {
                result = index + offset;
            }
            offset += it.tileSheet->getMaxIndex();
        }
        return result;
    }
}
