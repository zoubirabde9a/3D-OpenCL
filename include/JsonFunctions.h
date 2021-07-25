#ifndef JSONFUNCTIONS_H
#define JSONFUNCTIONS_H


#include <json\json.h>
#include <glm\glm.hpp>
#include "Vertex.h"
#include <QString>

namespace ZEngine {
    namespace json {
        extern Json::Value parseFile(const std::string &filePath);
        extern std::string write(const Json::Value &root, bool styled = true);
        extern float getFloat(const Json::Value& value, float defaultValue);
        extern int getInt(const Json::Value& value, int defaultValue);
        extern bool getBool(const Json::Value& value, bool defaultValue);
        extern double getDouble(const Json::Value& value, double defaultValue);
        extern glm::vec2 getVec2(const Json::Value& value, const glm::vec2 &defaultValue);
        extern glm::vec3 getVec3(const Json::Value& value, const glm::vec3 &defaultValue);
        extern glm::vec4 getVec4(const Json::Value& value, const glm::vec4 &defaultValue);
        extern ColorRGBA8 getColor(const Json::Value& value, const ColorRGBA8 &defaultValue);
        extern std::string getString(const Json::Value& value, const std::string &defaultValue);
        extern void writeToFile(const Json::Value &root, const std::string &filePath);
    }
}

#endif // JSONFUNCTIONS_H
