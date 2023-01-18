#ifndef TINYLOGGER_LOGGER_H
#define TINYLOGGER_LOGGER_H

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <cstring>
#include <filesystem>
#include <cstdarg>
#include <unistd.h>
#include <thread>
#include <queue>
#include <cmath>
#include <map>


class logger{

private:
    static std::string logFile;
    static std::string path;
    static void clearLogs();
    static std::string pName;
    static int maxSize;
    static int maxFiles;
    static std::fstream f;

    static void logLoop();
public:
    static void init(const std::string &name, int maxLogSize = 50, int maxFilesCount = 10);

    static void setThread(const std::string &name, std::thread::id threadID);

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