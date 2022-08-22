#include "logger.h"

std::string logger::logFile;
std::fstream logger::f;
std::string logger::path = "/var/log/"+pName;

void logger::init(std::string &name, int maxLogSize, int maxFilesCount){
    pName = name;
    maxSize = maxLogSize;
    maxFiles = maxFilesCount;
}

void logger::clearLogs(){
    DIR *dir;
    int logsCount = 0;
    struct dirent *ent;

    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;
            if(strstr(ent->d_name, ".log")){
                logsCount++;
            }
        }
        closedir(dir);
    }

    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;
            if(strstr(ent->d_name, ".log") && logsCount > maxFiles){
                std::string remFile = path;
                remFile += "/";
                remFile += ent->d_name;
                remove(remFile.c_str());
                logsCount--;
            }
        }
        closedir(dir);
    }
}

void logger::log(const std::string& formattedText, const std::string& color){
    time_t curr_time;
    tm * curr_tm;
    time(&curr_time);
    curr_tm = localtime(&curr_time);
    char dt[50];
    strftime(dt, 50, "%d-%m-%Y %H_%M_%S", curr_tm);

    if(logFile.empty())
        logFile =  std::string(dt) + ".log";

    if(access("/var/log", 06) != W_OK)
        path = "log/"+pName;

    if(!std::filesystem::is_directory(path))
        std::filesystem::create_directories(path);

    logFile = path+"/"+logFile;

    if(!f.is_open()){
        f.open(logFile.c_str(), std::ios::app);
        clearLogs();
    }else{
        f.seekg (0, std::ios::end);
        long size = f.tellg();
        if(size >= 1048576*maxSize){
            f.close();
            log(formattedText, color);
            return;
        }
    }

    strftime(dt, 50, "%d-%m-%Y %H:%M:%S", curr_tm);
    std::string str="["+std::string(dt)+formattedText;
    f << str.c_str() << std::endl;
    std::cout << color << str.c_str() << "\033[0m" << std::endl;
}


//strings
void logger::warn(const std::string& text){
    std::string format = " | WARN] ";
    log(format + text, "\033[0;33m");
}

void logger::error(const std::string& text){
    std::string format = " | ERROR] ";
    log(format + text, "\033[0;31m");
}

void logger::info(const std::string& text){
    std::string format = " | INFO] ";
    log(format + text, "\033[0;34m");
}

void logger::success(const std::string& text){
    std::string format = " | SUCCESS] ";
    log(format + text, "\033[0;32m");
}


//chars
void logger::warn(const char *text) {
    warn(std::string(text));
}

void logger::error(const char *text) {
    error(std::string(text));
}

void logger::info(const char *text) {
    info(std::string(text));
}

void logger::success(const char *text) {
    success(std::string(text));
}