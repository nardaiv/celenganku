#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

class Logger {
private:
    enum LogLevel {
        LOG_INFO,
        LOG_ERROR,
        LOG_DEBUG
    };

public:
    static void log(LogLevel level, const std::string& message) {
        std::time_t now = std::time(nullptr);
        std::string level_str;
        
        switch (level) {
            case LOG_INFO:
                level_str = "[INFO]";
                break;
            case LOG_ERROR:
                level_str = "[ERROR]";
                break;
            case LOG_DEBUG:
                level_str = "[DEBUG]";
                break;
        }

        std::stringstream ss;
        ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
        
        std::cout << ss.str() << " " << level_str << " " << message << std::endl;
    }

    static void info(const std::string& message) {
        log(LOG_INFO, message);
    }

    static void error(const std::string& message) {
        log(LOG_ERROR, message);
    }

    static void debug(const std::string& message) {
        log(LOG_DEBUG, message);
    }
}; 