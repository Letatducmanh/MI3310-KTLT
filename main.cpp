#include "ui/Console/ConsoleUI.h"
#include "services/SalesService.h"
#include "log/Logger.h"
#include <iostream>
#include <filesystem>

int main(int, char* argv[]) {
    // 1. Tính toán đường dẫn thư mục gốc của dự án
    std::filesystem::path rootPath = std::filesystem::path(argv[0]).parent_path().parent_path();
    
    // 2. Khởi tạo Logger với đường dẫn đó
    Logger::getInstance().initialize(rootPath.string());

    Logger::getInstance().log("Chuong trinh khoi dong.");
    
    try {
        SalesService salesService(argv[0]);
        ConsoleUI console(salesService);
        console.run();
    } 
    catch (const std::exception& e) {
        Logger::getInstance().log("LOI NGHIEM TRONG: " + std::string(e.what()));
        std::cerr << "Da xay ra loi khong mong muon: " << e.what() << std::endl;
        Logger::getInstance().shutdown();
        return 1;
    }
    catch (...) {
        Logger::getInstance().log("LOI NGHIEM TRONG: Loi khong xac dinh.");
        std::cerr << "Da xay ra loi khong xac dinh!" << std::endl;
        Logger::getInstance().shutdown(); 
        return 1;
    }

    Logger::getInstance().shutdown(); 
    return 0;
}