#include "logger.h"

std::string logger::pName;
int logger::maxSize;
int logger::maxFiles;
std::string logger::logFile;
std::string logger::path;
std::fstream logger::f;
std::string curThread;

time_t curr_time;
tm * curr_tm;
char dt[50];
struct timeval tv;
long milis;

std::queue<std::string> logs;
std::map<std::thread::id, std::string> threads;

void logger::logLoop(){
    while(true){
        while(!logs.empty()){
            time(&curr_time);
            curr_tm = localtime(&curr_time);
            bzero(dt, 50);
            strftime(dt, 50, "%d-%m-%Y %H_%M_%S", curr_tm);

            if(logFile.empty())
                logFile =  path + "/" +std::string(dt) + ".log";

            if(!f.is_open()) {
                f.open(logFile.c_str(), std::ios::app);
                clearLogs();
            }else {
                if(std::filesystem::exists(logFile)) {
                    uintmax_t size = std::filesystem::file_size(logFile);
                    if (size >= 1048576 * maxSize) {
                        f.close();
                        continue;
                    }
                }
            }
            std::string log = logs.front();

            f << log << std::endl;
            std::cout << log << std::endl;

            logs.pop();
        }
    }
}

void logger::init(const std::string &name, int maxLogSize, int maxFilesCount){
    pName = name;
    maxSize = maxLogSize;
    maxFiles = maxFilesCount;

    if(access("/var/log", 06) != W_OK)
        path = "log/"+pName;
    else
        path = "/var/log/"+pName;

    if(!std::filesystem::is_directory(path))
        std::filesystem::create_directories(path);

    std::thread logT(logLoop);
    logT.detach();
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
    time(&curr_time);
    curr_tm = localtime(&curr_time);
    gettimeofday(&tv, NULL);
    milis = lrint(tv.tv_usec/1000.0);
    if (milis>=1000) {
        milis -=1000;
        tv.tv_sec++;
    }

    curThread = "undefinedThread";
    if(threads.contains(std::this_thread::get_id()))
        curThread = threads[std::this_thread::get_id()];

    strftime(dt, 50, "%d-%m-%Y %H:%M:%S.", curr_tm);
    std::string log="["+std::string(dt)+std::to_string(milis) + " | " + curThread +formattedText;
    logs.push(color + log + "\033[0m");
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

void logger::setThread(const std::string &name, std::thread::id threadID) {
    threads.insert(std::make_pair(threadID, name));
}
