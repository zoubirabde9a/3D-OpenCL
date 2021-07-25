#ifndef TILESHEET_H
#define TILESHEET_H
#include "GLTexture.h"
#include <glm\glm.hpp>
#include <vector>

namespace ZEngine {
class TileSheet {
public:
    static glm::vec4 getUvs(const glm::ivec4 &Rect, const GLTexture* texture);
public:
    TileSheet(){}
    TileSheet(const std::string &filePath, const glm::ivec2 &tileDims, bool softFiletring);
    void init(const std::string &filePath, const glm::ivec2 &tileDims, bool softFiletring);
    void init(const std::string &filePath);
    void setDims(const glm::ivec2 &tileDims);
    int getMaxIndex() const;
    glm::vec4 getUvs(int index) const;
    int getIndex(const glm::vec4 &uvs) const;
    glm::vec4 getUvs(glm::ivec4 Rect) const;

public:
    void setTexture(const GLTexture* texture) { m_texture = texture; }
    GLuint getTextureId() const { return m_texture->id; }
    const GLTexture* getTexture() const { return m_texture; }
    const std::string &getFilePath() const { return m_texture->filePath; }
    const glm::vec2 &getTileDims() const { return m_tileDims; }

private:
    const GLTexture* m_texture;
    glm::ivec2 m_tileDims;
public:
    class Container {
    public:
        struct Slot {
            const TileSheet* tileSheet;
            std::string name;
        };
        struct Tile {
            const TileSheet* tileSheet = nullptr;
            GLuint index = -1;
        };
        void addTileSheet(const TileSheet* tileSheet, const std::string &name);
        Slot getTileSheet(const std::string &tileSheet);
        Tile getSlot(GLuint index) const;
        GLuint getIndex(const TileSheet* tileSheet, GLuint index)const;
        const std::vector < Slot > &getTileSheets() const { return m_tileSheets; }
    private:
        std::vector < Slot > m_tileSheets;
    };
};
}


#endif // TILESHEET_H
