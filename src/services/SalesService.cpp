#include "services/SalesService.h"
#include "log/Logger.h"
#include <filesystem>
#include <iostream>
#include <algorithm> 
#include <numeric>   

// --- HÀM DỰNG ĐÚNG ---
SalesService::SalesService(const char* exePath)
    // 1. Danh sách khởi tạo chỉ dùng để khởi tạo biến thành viên
    : dataPath(std::filesystem::path(exePath).parent_path().parent_path().string() + "/data"),
      dataManager(this->dataPath)
{
    // 2. Toàn bộ logic xử lý phải nằm trong thân hàm dựng (bên trong {})
    if (!std::filesystem::exists(this->dataPath)) {
        try {
            std::filesystem::create_directory(this->dataPath);
            Logger::getInstance().log("Thu muc 'data' khong ton tai. Da tu dong tao moi tai: " + this->dataPath);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Loi nghiem trong: Khong the tao thu muc 'data'. " << e.what() << std::endl;
        }
    }

    // Tải dữ liệu
    loadData();
    Logger::getInstance().log("Tai du lieu ban dau thanh cong.");
}

SalesService::~SalesService() {} 

void SalesService::loadData() {
    dataManager.loadAllData(products, invoices);
}

void SalesService::saveProductsData() const {
    // Chỉ lưu các file liên quan đến sản phẩm
    dataManager.saveAllData(this->products, this->invoices); 
}

void SalesService::saveInvoicesData() const {
    // Lưu các file liên quan đến hóa đơn
    dataManager.saveAllData(this->products, this->invoices); 
}

const std::string& SalesService::getDataPath() const {
    return this->dataPath;
}