//
// Created by jm on 4/28/24.
//

#ifndef JM_NGINX_LOG_H
#define JM_NGINX_LOG_H

#include <fstream>
#include <string>
#include <atomic>


// void ngx_log_init() 初始化日志, 打开或者创建日志文件
// ngx_log_error_core(int level, int error, const char *fmt, ...) 写日志文件
// ngx_vslprintf()
// 打开文件流, 向文件流中输入数据

#define Log log::getInstance()

extern int g_logLevel;

class log {
public:
    enum LogLevel {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        NUM_LOG_LEVELS
    };
    void ngx_log_init();
    std::fstream& stream(int logLevel);

public:
    log(const log& other) = delete;
    log& operator=(const log& other) = delete;
    static log& getInstance() {
        static log instance;
        return instance;
    }
private:
    log() {}
private:
    std::fstream m_fStream;
    std::string m_name;
//    std::atomic<int> m_count;
};

#define LOG_DEBUG if(g_logLevel <= log::DEBUG) \
    log::getInstance().stream(log::DEBUG)
#define LOG_INFO if(g_logLevel <= log::INFO) \
    log::getInstance().stream(log::INFO)
#define LOG_WARN log::getInstance().stream(log::WARN)
#define LOG_ERROR log::getInstance().stream(log::ERROR)
#endif //JM_NGINX_LOG_H
