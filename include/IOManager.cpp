#include "IOManager.h"
#include <fstream>
#include "ZEngineErrors.h"

namespace ZEngine {
    bool IOManager::readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer) {
        QFile file(QString(filePath.c_str()));
        file.open(QIODevice::ReadOnly);
        if (file.error() == QFile::NoError){
            QByteArray byteArray = file.readAll();
            const unsigned char* data = (const unsigned char*)byteArray.data();
            size_t fileSize = byteArray.size();
            buffer.resize(fileSize);
            for(size_t i = 0 ; i < fileSize; i++){
                buffer[i] = data[i];
            }
            return true;
        } else {
            fatalError("Read File Error : " + file.errorString().toStdString());
            return false;
        }
    }
    bool IOManager::readFileToBuffer(const std::string& filePath, std::vector<char>& buffer) {
        QFile file(filePath.c_str());
        file.open(QIODevice::ReadOnly);
        if (file.error() == QFile::NoError){
            QByteArray byteArray = file.readAll();
            const unsigned char* data = (const unsigned char*)byteArray.data();
            size_t fileSize = byteArray.size();
            buffer.resize(fileSize);
            for(size_t i = 0 ; i < fileSize; i++){
                buffer[i] = data[i];
            }
            return true;
        } else {
            fatalError("Read File Error : " + file.errorString().toStdString());
            return false;
        }
	}
    bool IOManager::readFileToBuffer(const std::string& filePath, std::string &buffer) {
        QFile file(QString(filePath.c_str()));
        file.open(QIODevice::ReadOnly);
        if (file.error() == QFile::NoError){
            QByteArray byteArray = file.readAll();
            const unsigned char* data = (const unsigned char*)byteArray.data();
            size_t fileSize = byteArray.size();
            buffer.resize(fileSize);
            for(size_t i = 0 ; i < fileSize; i++){
                buffer[i] = data[i];
            }
            return true;
        } else {
            fatalError("Read File Error : " + file.errorString().toStdString());
            return false;
        }
	}

    bool IOManager::writeToFileFromBuffer(const std::string& filePath, const std::string &buffer, IO mode) {
        QFile file(filePath.c_str());
        if (mode == IO::Override){
          if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
              fatalError("Error Opening File" + filePath + " :" + file.errorString().toStdString());
              return false;
          }
        } else {
            if (!file.open(QIODevice::WriteOnly)) {
            fatalError("Error Opening File" + filePath + " :" + file.errorString().toStdString());
            return false;
         }
        }
        file.write(buffer.c_str(), buffer.size());
        return true;
	}
     bool IOManager::writeToFileFromBuffer(const std::string& filePath, const std::list < std::string > &container, IO mode) {

         QFile file(filePath.c_str());
         if (mode == IO::Override){
           if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
               fatalError("Error Opening File" + filePath + " :" + file.errorString().toStdString());
               return false;
           }
         } else {
             if (!file.open(QIODevice::WriteOnly)) {
             fatalError("Error Opening File" + filePath + " :" + file.errorString().toStdString());
             return false;
          }
         }
         for (auto &it : container){
              file.write(it.c_str(), it.size());
              file.write("\n", 1);
         }

         return true;
	}

}
