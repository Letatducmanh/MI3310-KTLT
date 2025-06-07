#include "log/Logger.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <filesystem>

Logger* Logger::instance = nullptr;

Logger& Logger::getInstance() {
    if (instance == nullptr) {
        instance = new Logger();
    }
    return *instance;
}

// Hàm dựng giờ chỉ đơn giản là khởi tạo
Logger::Logger() : isInitialized(false) {}

// Hàm hủy không làm gì cả, việc đóng file sẽ do shutdown() đảm nhiệm
Logger::~Logger() {}

// Hàm thiết lập chính
void Logger::initialize(const std::string& projectRootPath) {
    if (isInitialized) return; // Chỉ cho phép initialize một lần

    std::string logDir = projectRootPath + "/log";
    if (!std::filesystem::exists(logDir)) {
        std::filesystem::create_directory(logDir);
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    std::string fileName = logDir + "/log_" + ss.str() + ".txt";

    logFile.open(fileName, std::ios_base::app);
    if (!logFile.is_open()) {
        std::cerr << "Loi: Khong the mo file log de ghi." << std::endl;
    } else {
        isInitialized = true;
        log("He thong Log da san sang.");
    }
}

void Logger::shutdown() {
    if (isInitialized && logFile.is_open()) {
        log("Chuong trinh ket thuc.");
        logFile.close();
        isInitialized = false;
    }
}

// Hàm ghi log cần kiểm tra đã initialize chưa
void Logger::log(const std::string& message) {
    if (!isInitialized || !logFile.is_open()) return;

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    logFile << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "] ";
    logFile << "[INFO] " << message << std::endl;
}