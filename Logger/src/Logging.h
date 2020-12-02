#ifndef LOGGER_H
#define LOGGER_H
#include <string.h>
#include <stdarg.h>

void log(const char * format, ...);

#define FORMAT "\n\t\t\t[%s() in %s(line %d)]\n"


#ifdef _MSC_VER
    #define LOG_MACRO(format,...) log(format FORMAT,__VA_ARGS__,__FUNCTION__,strrchr(__FILE__,'\\')+1,__LINE__)
    #define LOG_ERROR(format,...) LOG_MACRO("[ERR] " format,__VA_ARGS__)
    #define LOG_INFO(format,...) LOG_MACRO("[INF] " format,__VA_ARGS__)
    #define LOG_WARNING(format,...) LOG_MACRO("[WRN] " format,__VA_ARGS__)
    #define LOG_DEBUG(format,...) LOG_MACRO("[DBG] " format,__VA_ARGS__)
#elif defined(__GNUC__)
    #define LOG_MACRO(format,...) log(format FORMAT,##__VA_ARGS__,__PRETTY_FUNCTION__,strrchr(__FILE__,'/')+1,__LINE__)
    #define LOG_ERROR(format,...) LOG_MACRO("[ERR] " format,##__VA_ARGS__)
    #define LOG_INFO(format,...) LOG_MACRO("[INF] " format,##__VA_ARGS__)
    #define LOG_WARNING(format,...) LOG_MACRO("[WRN] " format,##__VA_ARGS__)
    #define LOG_DEBUG(format,...) LOG_MACRO("[DBG] " format,##__VA_ARGS__)
#else
    #define LOG_MACRO(format,...)
    #define LOG_ERROR(format,...) LOG_MACRO("[ERR] " format,__VA_ARGS__)
    #define LOG_INFO(format,...) LOG_MACRO("[INF] " format,__VA_ARGS__)
    #define LOG_WARNING(format,...) LOG_MACRO("[WRN] " format,__VA_ARGS__)
    #define LOG_DEBUG(format,...) LOG_MACRO("[DBG] " format,__VA_ARGS__)
#endif




#endif
