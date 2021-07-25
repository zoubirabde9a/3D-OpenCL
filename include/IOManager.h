#pragma once

#include <vector>
#include <list>
#include <QFile>>

namespace ZEngine {
	enum class IO {
		Write, Override
	};
	struct DirEntry {
		std::string path;
		bool isDirectory;
	};
    class IOManager
    {
    public:
        static bool readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);
        static bool readFileToBuffer(const std::string& filePath, std::vector<char>& buffer);
        static bool readFileToBuffer(const std::string& filePath, std::string &buffer);
        static bool writeToFileFromBuffer(const std::string& filePath, const std::string &buffer, IO mode = IO::Override);
        static bool writeToFileFromBuffer(const std::string& filePath, const std::list < std::string > &buffer, IO mode = IO::Override);
    };

}
