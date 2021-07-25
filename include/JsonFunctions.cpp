#include "JsonFunctions.h"
#include <fstream>
#include "IOManager.h"
#include "ZEngineErrors.h"

// Doc https://en.wikibooks.org/wiki/JsonCpp
// doc http://www.thomaswhitton.com/blog/2013/06/28/json-c-plus-plus-examples/
namespace ZEngine {
    namespace json {
        using namespace Json;
        Value parseFile(const QString &filePath) {
            QFile file(filePath);
            file.open((QIODevice::ReadOnly));
            QByteArray byteArray = file.readAll();
            const  char* imageData = (const char*)byteArray.data();
            size_t dataSize = byteArray.size();

            //std::ifstream ifs(filePath);
            Reader reader(Features::strictMode());
            Value root;
            if (!reader.parse(imageData, imageData + dataSize, root, false)) {
                fatalError(filePath.toStdString() + ": " + reader.getFormattedErrorMessages());
            }
            return root;
        }
        std::string write(const Value &root, bool styled) {
            if (styled) {
                Json::StyledWriter writer;
                return writer.write(root);
            }
            else {
                Json::FastWriter writer;
                return writer.write(root);
            }
        }
        void writeToFile(const Value &root, const std::string &filePath) {
            IOManager::writeToFileFromBuffer(filePath, write(root).c_str());
        }
        float getFloat(const Json::Value& value, float defaultValue) {
            if (value.isNumeric()) {
                return value.asFloat();
            }
            else return defaultValue;
        }
        int getInt(const Json::Value& value, int defaultValue) {
            if (value.isNumeric()) {
                return value.asInt();
            }
            else return defaultValue;
        }
        bool getBool(const Json::Value& value, bool defaultValue) {
            if (value.isBool()) {
                return value.asBool();
            }
            else return defaultValue;
        }
        double getDouble(const Json::Value& value, double defaultValue) {
            if (value.isNumeric()) {
                return value.asDouble();
            }
            else return defaultValue;
        }
        std::string getString(const Json::Value& value, const std::string &defaultValue) {
            if (value.isString()) {
                return value.asString();
            }
            else return defaultValue;
        }
        glm::vec2 getVec2(const Json::Value& value, const glm::vec2 &dv) {
            if (value.isArray() && value.size() >= 2) {
                return glm::vec2(getFloat(value[0], dv.x), getFloat(value[1], dv.y));
            }
            else return dv;
        }
        glm::vec3 getVec3(const Json::Value& value, const glm::vec3 &dv) {
            if (value.isArray() && value.size() >= 3) {
                return glm::vec3(getFloat(value[0], dv.x), getFloat(value[1], dv.y), getFloat(value[2], dv.z));
            }
            else return dv;
        }
        glm::vec4 getVec4(const Json::Value& value, const glm::vec4 &dv) {
            if (value.isArray() && value.size() >= 4) {
                return glm::vec4(getFloat(value[0], dv.x), getFloat(value[1], dv.y),
                    getFloat(value[2], dv.z), getFloat(value[3], dv.w));
            }
            else return dv;
        }
        ColorRGBA8 getColor(const Json::Value &value, const ColorRGBA8 &dv) {
            if (value.isArray() && value.size() >= 4) {
                ColorRGBA8 color;
                color.r = (GLubyte)json::getInt(value[0], dv.r);
                color.g = (GLubyte)json::getInt(value[1], dv.g);
                color.b = (GLubyte)json::getInt(value[2], dv.b);
                color.a = (GLubyte)json::getInt(value[3], dv.a);
                return color;
            }
            return dv;
        }
    }
}
