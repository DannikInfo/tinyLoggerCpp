#ifndef TINYLOGGER_LOGGER_H
#define TINYLOGGER_LOGGER_H

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <chrono>
#include <ctime>
#include <cstring>
#include <filesystem>
#include <cstdarg>
#include <unistd.h>

class logger {
private:
    static std::string logFile;
    static std::string path;
    static void clearLogs();
    static std::string pName;
    static int maxSize;
    static int maxFiles;
public:
    static std::fstream f;

    static void init(std::string &name, int maxLogSize = 50, int maxFilesCount = 10);

    static void log(const std::string& text, const std::string& color);

    static void warn(const std::string& text);
    static void error(const std::string& text);
    static void info(const std::string& text);
    static void success(const std::string& text);

    //char* formats
    static void warn(const char* text);
    static void error(const char* text);
    static void info(const char* text);
    static void success(const char* text);
};

#endif //TINYLOGGER_LOGGER_H