#include "persistence/DataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

// Triển khai hàm dựng mới
// hàm dựng chi tiết về đối tượng DataManager 
DataManager::DataManager(const std::string& dataDirectory)
    : productsFile(dataDirectory + "/products.txt"),      
      invoicesFile(dataDirectory + "/invoices.txt"),      
      ordersFile(dataDirectory + "/orders.txt"),          
      invoiceLogFile(dataDirectory + "/invoices_log.txt")
{}
// hàm về load dữ liệu 
void DataManager::loadAllData(std::vector<Product>& products, std::vector<Invoice>& invoices) {
    loadProducts(products);
    loadInvoices(invoices);
    loadOrderItems(invoices, products);
    for(auto& inv : invoices) {
        inv.calculateTotalAmount();
    }
}
// hàm về lưu trữ dữ liệu 
void DataManager::saveAllData(const std::vector<Product>& products, const std::vector<Invoice>& invoices) {
    saveProducts(products);
    saveInvoices(invoices);
    saveOrderItems(invoices);
}


std::pair<std::vector<Product>, ImportResult> DataManager::loadProductsFromImportFile(const std::string& filename, const std::vector<Product>& existingProducts) {
    std::vector<Product> newProducts;
    ImportResult result;

    std::ifstream file(filename);
    if (!file.is_open()) {
        return {newProducts, result}; // Trả về rỗng nếu không mở được file
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, name, unit, priceStr, quantityStr;
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, unit, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, quantityStr);

        if (id.empty() || name.empty() || unit.empty() || priceStr.empty() || quantityStr.empty()) {
            result.missingFieldCount++;
            continue;
        }

        bool isDuplicate = false;
        for (const auto& p : existingProducts) {
            if (p.getProductId() == id) {
                isDuplicate = true;
                break;
            }
        }
        if (isDuplicate) {
            result.duplicateCount++;
            continue;
        }
        
        try {
            double price = std::stod(priceStr);
            int quantity = std::stoi(quantityStr);
            newProducts.emplace_back(id, name, unit, price, quantity);
            result.importedCount++;
        } catch(...) {
            result.missingFieldCount++; // Coi như lỗi định dạng là thiếu trường
        }
    }
    return {newProducts, result};
}

void DataManager::appendInvoiceToLog(const Invoice& invoice){
    std::ofstream file(invoiceLogFile, std::ios::app);
    if(!file.is_open()) return;
    file << getInvoiceStringForLog(invoice) << "\n\n";
}


void DataManager::loadProducts(std::vector<Product>& products) {
    std::ifstream file(productsFile);
    if (!file.is_open()) return;

    products.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, name, unit, priceStr, quantityStr;
        
        // Tách các trường ra
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, unit, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, quantityStr);

        if (!id.empty() && !name.empty() && !unit.empty() && !priceStr.empty() && !quantityStr.empty()) {
            try {
                // Dùng try-catch để phòng trường hợp dữ liệu số không hợp lệ
                double price = std::stod(priceStr);
                int quantity = std::stoi(quantityStr);
                products.emplace_back(id, name, unit, price, quantity);
            } catch (const std::invalid_argument& e) {
                // Bỏ qua dòng bị lỗi định dạng số
                std::cerr << "Warning: Loi dinh dang so o file products.txt, bo qua dong: " << line << std::endl;
            }
        }
    }
}

void DataManager::saveProducts(const std::vector<Product>& products) {
    std::ofstream file(productsFile);
    if (!file.is_open()) {
        std::cout << "Khong the mo file de ghi san pham! " << std::endl;
        return;
    }
    for (const auto& p : products) {
        file << p.getProductId() << "," << p.getProductName() << "," << p.getUnit() << ","
             << p.getPrice() << "," << p.getQuantity() << "\n";
    }
}

void DataManager::loadInvoices(std::vector<Invoice>& invoices) {
    std::ifstream file(invoicesFile);
    if (!file.is_open()) return;
    invoices.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, date;
        std::getline(ss, id, ',');
        std::getline(ss, date);
        if(!id.empty()){
            invoices.emplace_back(id, date);
        }
    }
}

void DataManager::saveInvoices(const std::vector<Invoice>& invoices) {
    std::ofstream file(invoicesFile);
    if (!file.is_open()) {
        std::cout << "Khong the mo file de ghi hoa don! " << std::endl;
        return;
    }

    for (const auto& inv : invoices) {
        file << inv.getInvoiceId() << "," << inv.getDate() << "\n";
    }
}

void DataManager::loadOrderItems(std::vector<Invoice>& invoices, const std::vector<Product>& allProducts) {
    std::ifstream file(ordersFile);
    if (!file.is_open()) return;
    
    std::string line;
    Invoice* currentInvoice = nullptr;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string tag, id;
        std::getline(ss, tag, ',');

        if (tag == "INVOICE") {
            std::getline(ss, id);
            currentInvoice = nullptr;
            for (auto& inv : invoices) {
                if (inv.getInvoiceId() == id) {
                    currentInvoice = &inv;
                    // Không cần clearItems vì các invoice này được tạo mới và đang rỗng
                    break;
                }
            }
        } else if (tag == "ITEM" && currentInvoice != nullptr) {
            std::string pid, pname, priceStr, qtyStr;
            std::getline(ss, pid, ',');
            std::getline(ss, pname, ',');
            std::getline(ss, priceStr, ',');
            std::getline(ss, qtyStr);

            const Product* productTemplate = nullptr;
            for(const auto& p : allProducts) {
                if (p.getProductId() == pid) {
                    productTemplate = &p;
                    break;
                }
            }

            if(productTemplate) {
                // Nếu sản phẩm vẫn còn tồn tại, dùng thông tin mới nhất
                currentInvoice->addItem(OrderItem(*productTemplate, std::stoi(qtyStr)));
            } else {
                Product historicalProduct(pid, pname, "", std::stod(priceStr), 0);
                currentInvoice->addItem(OrderItem(historicalProduct, std::stoi(qtyStr)));
            }
        }
    }
}

void DataManager::saveOrderItems(const std::vector<Invoice>& invoices) {
    std::ofstream file(ordersFile);
    if (!file.is_open()) return;

    for (const auto& inv : invoices) {
        file << "INVOICE," << inv.getInvoiceId() << "\n";
        for (const auto& item : inv.getItems()) {
            const auto& p = item.getProduct();
            // LƯU THÊM TÊN VÀ GIÁ VÀO FILE
            file << "ITEM," << p.getProductId() << "," << p.getProductName() << "," << p.getPrice() << "," << item.getQuantity() << "\n";
        }
    }
}

std::string DataManager::getInvoiceStringForLog(const Invoice& invoice) {
    std::ostringstream oss;
    oss << "=================================== HOA DON ====================================\n";
    oss << "Ma hoa don: " << invoice.getInvoiceId() << "\n";
    oss << "Ngay lap: " << invoice.getDate() << "\n";
    oss << "================================================================================\n";
    oss << std::left << std::setw(10) << "Ma SP"
        << std::setw(25) << "Ten SP"
        << std::setw(10) << "So luong"
        << std::setw(15) << "Don gia"
        << std::setw(15) << "Thanh tien" <<"\n";
    oss << "--------------------------------------------------------------------------------\n";
    for (const auto& item : invoice.getItems()) {
        const auto& p = item.getProduct();
        oss << std::left << std::setw(10) << p.getProductId()
            << std::setw(25) << p.getProductName()
            << std::setw(10) << item.getQuantity()
            << std::setw(15) << std::fixed << std::setprecision(0) << p.getPrice()
            << std::setw(15) << std::fixed << std::setprecision(0) << item.getTotalPrice() << "\n";
    }
    oss << "--------------------------------------------------------------------------------\n";
    oss << "TONG TIEN: " << std::fixed << std::setprecision(0) << invoice.getTotalAmount() << " VND\n";
    oss << "================================================================================\n";
    return oss.str();
}