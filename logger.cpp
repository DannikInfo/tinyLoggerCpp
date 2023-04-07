#include "logger.h"

std::string logger::pName;
int logger::maxSize;
int logger::maxFiles;
std::mutex logger::mutex;
std::string logger::logFile;
std::string logger::path;
std::fstream logger::f;
std::string curThread;

//LogFile times
time_t curr_timeLF;
tm * curr_tmLF;
char dtLF[50];

//Log string times
time_t curr_timeLStr;
tm * curr_tmLStr;
char dtLStr[50];

struct timeval tv;
long milis;

int minDepth = 0;

std::queue<std::string> logs;
std::map<std::thread::id, std::string> threads;

void logger::logLoop(){
    std::string log;
    while(true){
        time(&curr_timeLF);
        curr_tmLF = localtime(&curr_timeLF);
        bzero(dtLF, 50);
        strftime(dtLF, 50, "%d-%m-%Y %H_%M_%S", curr_tmLF);

        if(logFile.empty())
            logFile =  path + "/" +std::string(dtLF) + ".log";

        if(!f.is_open()) {
            clearLogs();
            f.open(logFile.c_str(), std::ios::app);
        }else {
            if(std::filesystem::exists(logFile)) {
                uintmax_t size = std::filesystem::file_size(logFile);
                if (size >= 1048576 * maxSize) {
                    f.close();
                    logFile.clear();
                    continue;
                }
            }
        }

        while(!logs.empty()){
            mutex.lock();
            log = logs.front();
            f << log << std::endl;
            std::cout << log << std::endl;
            logs.pop();
            mutex.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

void logger::log(const std::string& formattedText, const std::string& color, int &depth) {
    depth++;
    time(&curr_timeLStr);
    curr_tmLStr = localtime(&curr_timeLStr);
    gettimeofday(&tv, NULL);
    milis = lrint(tv.tv_usec / 1000.0);
    if (milis >= 1000) {
        milis -= 1000;
        tv.tv_sec++;
    }

    curThread = "undefinedThread";
    if (threads.contains(std::this_thread::get_id()))
        curThread = threads[std::this_thread::get_id()];

    strftime(dtLStr, 50, "%d-%m-%Y %H:%M:%S.", curr_tmLStr);
    std::string logStr = "[" + std::string(dtLStr) + std::to_string(milis) + " | " + curThread + formattedText;

    try {
        mutex.lock();
        logs.push(color + logStr + "\033[0m");
        mutex.unlock();
    }catch(std::exception &e){
        std::cout << "\033[0;31m" << "[" << std::string(dtLF) << std::to_string(milis) << " | " + curThread << " | ERROR] " << e.what() << std::endl;
        if(depth < 5)
            log(formattedText, color, depth);
    }
    depth--;
}

//strings
void logger::warn(const std::string& text){
    std::string format = " | WARN] ";
    log(format + text, "\033[0;33m", minDepth);
}

void logger::error(const std::string& text){
    std::string format = " | ERROR] ";
    log(format + text, "\033[0;31m", minDepth);
}

void logger::info(const std::string& text){
    std::string format = " | INFO] ";
    log(format + text, "\033[0;34m", minDepth);
}

void logger::success(const std::string& text){
    std::string format = " | SUCCESS] ";
    log(format + text, "\033[0;32m", minDepth);
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
