#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger {
public:
    // Hàm static để lấy đối tượng Logger duy nhất
    static Logger& getInstance();

    // Hàm mới để thiết lập Logger với đường dẫn gốc
    void initialize(const std::string& projectRootPath);
    
    // Hàm mới để đóng log một cách tường minh
    void shutdown();

    // Hàm để ghi một thông điệp vào file log
    void log(const std::string& message);

    // Xóa các hàm tạo bản sao để đảm bảo tính duy nhất (singleton)
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;

private:
    // Hàm dựng private để không thể tạo đối tượng từ bên ngoài
    Logger();
    ~Logger();

    std::ofstream logFile;
    bool isInitialized = false; // Thêm cờ để kiểm tra đã initialize chưa
    static Logger* instance;
};

#endif //LOGGER_H