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
#include <mutex>
#include <sys/stat.h>
#include <algorithm>


class logger{

private:
    static std::string logFile;
    static std::string path;
    static void clearLogs();
    static std::string pName;
    static int maxSize;
    static int maxFiles;
    static std::fstream f;

    static int minDepth;
    static std::mutex mutex;
    static void logLoop();

    static std::string stringConstructor(){
        return "";
    }

    template <typename T, typename... Args>
    static std::string stringConstructor(T text, Args... args){
        std::ostringstream oss;
        oss << text << " " << stringConstructor(args...);
        return oss.str();
    }
public:
    static void init(const std::string &name, int maxLogSize = 50, int maxFilesCount = 10);

    static void setThread(const std::string &name, std::thread::id threadID);

    static void log(const std::string& text, const std::string& color, int &depth);

    /**
     * @deprecated
     * @param text
     */
    static void warn(const std::string& text);
    /**
     * @deprecated
     * @param text
     */
    static void error(const std::string& text);
    /**
     * @deprecated
     * @param text
     */
    static void info(const std::string& text);
    /**
     * @deprecated
     * @param text
     */
    static void success(const std::string& text);

    template <typename T, typename... Args>
    static void warn(T text, Args... args){
        std::string format = " | WARN] ";
        std::ostringstream oss;
        oss << text << " " << stringConstructor(args...);
        log(format + oss.str(), "\033[0;33m", minDepth);
    }

    template <typename T, typename... Args>
    static void error(T text, Args... args){
        std::string format = " | ERROR] ";
        std::ostringstream oss;
        oss << text << " " << stringConstructor(args...);
        log(format + oss.str(), "\033[0;31m", minDepth);
    }

    template <typename T, typename... Args>
    static void info(T text, Args... args){
        std::string format = " | INFO] ";
        std::ostringstream oss;
        oss << text << " " << stringConstructor(args...);
        log(format + oss.str(), "\033[0;34m", minDepth);
    }

    template <typename T, typename... Args>
    static void success(T text, Args... args){
        std::string format = " | SUCCESS] ";
        std::ostringstream oss;
        oss << text << " " << stringConstructor(args...);
        log(format + oss.str(), "\033[0;32m", minDepth);
    }

    //char* formats
    /**
     * @deprecated
     * @param text
     */
    static void warn(const char* text);
    /**
     * @deprecated
     * @param text
     */
    static void error(const char* text);
    /**
     * @deprecated
     * @param text
     */
    static void info(const char* text);
    /**
     * @deprecated
     * @param text
     */
    static void success(const char* text);

};

#endif //TINYLOGGER_LOGGER_H