#pragma once

#include <list>
#include <thread>
#include <iostream>
#include <string>

#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

#include <mutex>
#include <fstream>

#define OUTPUT_STRING(x)    \
    va_list ap;             \
    char buf[8192];         \
    va_start(ap, f);        \
    vsprintf(buf, f, ap);   \
    va_end(ap);             \
    std::lock_guard<std::mutex> scoped(__this_mutex__); \
    std::cout << "[" << std::this_thread::get_id() << "] " << timestr() << x << buf << std::endl;   \
    if (fs_) { *fs_ << "[" << std::this_thread::get_id() << "] " << timestr() << x << buf << std::endl; \
        if (++written_count_ > rotate_max_) { rotate(); }   \
        }
class logger {
    
    std::mutex __this_mutex__;
    std::shared_ptr<std::ofstream> fs_;
    std::list<std::string> file_id_;
    uint32_t rotate_max_;
    uint32_t written_count_;
    
public:
    logger(){}
    ~logger(){}
    
public:
    static logger& instance() {
        static logger __instance__;
        return __instance__;
    }
    
private:
    std::string timestr() {
        
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        
        struct tm *p = gmtime(&tv.tv_sec);
        char sbuf[1024];
        return std::move(std::string(sbuf, strftime(sbuf, sizeof(sbuf), "[%Y-%m-%d %H:%M:%S]", p)));
    }
    
    void rotate() {
        written_count_ = 0;
        instance().fs_ = std::make_shared<std::ofstream>(instance().file_id_.front().c_str(), std::ofstream::out | std::ios::app);
        instance().file_id_.push_back(instance().file_id_.front());
        instance().file_id_.pop_front();
    }
    
public:
#if defined(ENABLE_TRACE) || defined(ENABLE_DEBUG) || defined(ENABLE_INFO) || defined(ENABLE_WARN) || defined(ENABLE_ERR) || defined(ENABLE_FATAL)
    static void init(/*const std::string folder, */const std::string fname, int rotate_times, int rotate_file_number) {
        
        instance().rotate_max_ = rotate_times;
        for (auto i = 0; i < rotate_file_number; ++i) {
            char ss[256];
            memset(ss, 0, sizeof(ss));
//            sprintf(ss, "%s/%d-%s", folder.c_str(), i, fname.c_str());
			sprintf(ss, "%d-%s", i, fname.c_str());
            instance().file_id_.push_back(ss);
        }
        
        instance().rotate();
        
    }
#else
    static void init(const std::string fname, int rotate_times, int rotate_file_number) {}
#endif
    
    void trace(const char *f, ...) {
        OUTPUT_STRING(" [TRACE] ");
    }
    
    void debug(const char *f, ...) {
        OUTPUT_STRING(" [DEBUG] ");
    }
    
    void info(const char *f, ...) {
        OUTPUT_STRING(" [INFO] ");
    }
    
    void warn(const char *f, ...) {
        OUTPUT_STRING(" [WARN] ");
    }
    
    void error(const char *f, ...) {
        OUTPUT_STRING(" [ERR] ");
    }
    
    void fatal(const char *f, ...) {
        OUTPUT_STRING(" [FATAL] ");
    }
    

};


#if defined(ENABLE_TRACE)
#define print_trace(...)  do { logger::instance().trace(__VA_ARGS__);  } while(0);
#else
#define print_trace(...)
#endif

#if defined(ENABLE_DEBUG)
#define print_debug(...)  do { logger::instance().debug(__VA_ARGS__);  } while(0);
#else
#define print_debug(...)
#endif

#if defined(ENABLE_INFO)
#define print_info(...)  do { logger::instance().info(__VA_ARGS__);  } while(0);
#else
#define print_info(...)
#endif

#if defined(ENABLE_WARN)
#define print_warn(...)  do { logger::instance().warn(__VA_ARGS__);  } while(0);
#else
#define print_warn(...)
#endif

#if defined(ENABLE_ERROR)
#define print_error(...)  do { logger::instance().error(__VA_ARGS__);  } while(0);
#else
#define print_error(...)
#endif

#if defined(ENABLE_FATAL)
#define print_fatal(...)  do { logger::instance().fatal(__VA_ARGS__);  } while(0);
#else
#define print_fatal(...)
#endif



