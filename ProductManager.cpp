#include "SalesManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits> 
#include <algorithm>

// Product management
void SalesManager::addProduct() {
    if (productCount >= maxProducts) {
        resizeProductArray();
    }
    
    std::cout << "\n=== THEM SAN PHAM MOI ===" << std::endl;
    products[productCount].inputProduct(products, productCount);
    productCount++;
    std::cout << "Them san pham thanh cong!" << std::endl;
    saveProductsToFile();
}

void SalesManager::editProduct() {
    char id[20];
    Product* product = nullptr;

    do {
        std::cout << "\nNhap ma san pham can sua: ";
        std::cin >> id;
        product = findProductById(id);
        if (product == nullptr) {
            std::cout << "Khong tim thay san pham voi ma: " << id << ". Vui long nhap lai!" << std::endl;
        }
    } while (product == nullptr);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\n === THONG TIN HIEN TAI: ===" << std::endl;
    std::cout << std::left << std::setw(10) << "Ma SP"
              << std::setw(25) << "Ten SP"
              << std::setw(10) << "Don vi"
              << std::setw(15) << "Don gia" 
              << std::setw(15) << "So luong" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    
    product->displayProduct();

    std::cout << "\nNhap thong tin moi (nhap 0 de giu nguyen):" << std::endl;

    std::string newId, newName, newUnit;
    double newPrice;
    int newQuantity;

    // Hàm kiểm tra chuỗi chỉ chứa số
    auto isNumber = [](const std::string& s) -> bool {
        return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
    };

    // Nhập mã sản phẩm mới
    do {
        std::cout << "Ma san pham (" << product->getProductId() << "): ";
        std::getline(std::cin, newId);

        // Nếu giữ nguyên hoặc không nhập gì thì thoát vòng lặp
        if (newId.empty() || newId == "0") break;

        // Kiểm tra mã sản phẩm phải là số nguyên
        if (!isNumber(newId)) {
            std::cout << "Ma san pham chi duoc phep la so nguyen! Vui long nhap lai." << std::endl;
            continue;
        }

        // Kiểm tra trùng mã với sản phẩm khác
        Product* existed = findProductById(newId.c_str());
        if (existed != nullptr && existed != product) {
            std::cout << "Ma san pham da ton tai, vui long nhap ma khac!" << std::endl;
        } else {
            product->setProductId(newId.c_str());
            break;
        }
    } while (true);

    // Nhập tên sản phẩm mới
    std::cout << "Ten san pham (" << product->getProductName() << "): ";
    std::getline(std::cin, newName);
    if (!newName.empty() && newName != "0") {
        product->setProductName(newName.c_str());   
    }

    // Nhập đơn vị mới
    std::cout << "Don vi (" << product->getUnit() << "): ";
    std::getline(std::cin, newUnit);
    if (!newUnit.empty() && newUnit != "0") {
        product->setUnit(newUnit.c_str());
    }

    // Nhập đơn giá mới
    std::cout << "Don gia (" << product->getPrice() << "): ";
    std::string priceInput;
    std::getline(std::cin, priceInput);
    if (!priceInput.empty() && priceInput != "0") {
        newPrice = std::stod(priceInput);
        product->setPrice(newPrice);
    }

    // Nhập số lượng mới
    std::cout << "So luong (" << product->getQuantity() << "): ";
    std::string quantityInput;
    std::getline(std::cin, quantityInput);
    if (!quantityInput.empty() && quantityInput != "0") {
        newQuantity = std::stoi(quantityInput);
        product->setQuantity(newQuantity);
    }

    std::cout << "Cap nhat san pham thanh cong!" << std::endl;
    saveProductsToFile();
}

void SalesManager::deleteProduct() {
    char id[20];
    char choice;
    do {
        std::cout << "\nNhap ma san pham can xoa: ";
        std::cin >> id;
        
        bool found = false;
        for (int i = 0; i < productCount; i++) {
            if (strcmp(products[i].getProductId(), id) == 0) {
                for (int j = i; j < productCount - 1; j++) {
                    products[j] = products[j + 1];
                }
                productCount--;
                std::cout << "Xoa san pham thanh cong!" << std::endl;
                saveProductsToFile();
                found = true;
                break;
            }
        }
        if (found) break;

        std::cout << "Khong tim thay san pham voi ma: " << id << std::endl;
        std::cout << "Ban co muon tiep tuc xoa san pham khac? (y/n): ";
        std::cin >> choice;
        if (choice == 'n' || choice == 'N') break;
    } while (true);
}

void SalesManager::displayAllProducts() const {
    if (productCount == 0) {
        std::cout << "\nKhong co san pham nao trong he thong!" << std::endl;
        return;
    }
    
    std::cout << "\n=== DANH SACH SAN PHAM ===" << std::endl;
    std::cout << std::left << std::setw(10) << "Ma SP"
              << std::setw(25) << "Ten SP"
              << std::setw(10) << "Don vi"
              << std::setw(15) << "Don gia" 
              << std::setw(15) << "So luong" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    
    for (int i = 0; i < productCount; i++) {
        products[i].displayProduct();
    }
    std::cout << std::endl;
}

Product* SalesManager::findProductById(const char* id) const {
    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].getProductId(), id) == 0) {
            return &products[i];
        }
    }
    return nullptr;
}

void SalesManager::searchProductById() const {
    while (true) {
        char id[20];
        std::cout << "\nNhap ma san pham can tim: ";
        std::cin >> id;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

        bool found = false;
        for (int i = 0; i < productCount; i++) {
            if (strcmp(products[i].getProductId(), id) == 0) {
                std::cout << "\n=== THONG TIN SAN PHAM ===" << std::endl;
                std::cout << std::left << std::setw(10) << "Ma SP"
                          << std::setw(25) << "Ten SP"
                          << std::setw(10) << "Don vi"
                          << std::setw(15) << "Don gia"
                          << std::setw(15) << "So luong" << std::endl;
                std::cout << "------------------------------------------------------------" << std::endl;
                products[i].displayProduct();
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "Khong tim thay san pham voi ma: " << id << std::endl;
            std::cout << "Ban co muon tiep tuc su dung chuc nang nay khong? (y/n): ";
            char choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice == 'y' || choice == 'Y') {
                continue;
            } else {
                break;
            }
        } 
        else {
            break;
        }    
    }
}

void SalesManager::searchProductByName() const {
    while (true) {
        std::string name;
        std::cout << "\nNhap ten san pham can tim: ";
        std::getline(std::cin, name);

        bool found = false;
        std::cout << "\n=== KET QUA TIM KIEM ===" << std::endl;
        std::cout << std::left << std::setw(10) << "Ma SP"
                  << std::setw(25) << "Ten SP"
                  << std::setw(10) << "Don vi"
                  << std::setw(15) << "Don gia"
                  << std::setw(15) << "So luong" << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;

        for (int i = 0; i < productCount; i++) {
            std::string prodName = products[i].getProductName();
            // So sánh không phân biệt hoa thường
            std::string prodNameLower = prodName, nameLower = name;
            std::transform(prodNameLower.begin(), prodNameLower.end(), prodNameLower.begin(), ::tolower);
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

            if (prodNameLower.find(nameLower) != std::string::npos) {
                products[i].displayProduct();
                found = true;
            }
        }

        if (!found) {
            std::cout << "Khong tim thay san pham voi ten: " << name << std::endl;
            std::cout << "Ban co muon tiep tuc tim kiem khong? (y/n): ";
            char choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice != 'y' && choice != 'Y') break;
        }
        else {
            break;
        }
        
    }
}

void SalesManager::importProductsFromFile(const char* filename) {
    while (true) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Khong the mo file " << filename << " de doc du lieu!" << std::endl;
            std::cout << "Ban co muon tiep tuc chuc nang nay khong? (y/n): ";
            char choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice == 'y' || choice == 'Y') {
                std::cout << "Nhap lai ten file can import: ";
                char newFilename[100];
                std::cin.getline(newFilename, 100);
                filename = newFilename;
                continue;
            } else {
                return;
            }
        }

        int importedCount = 0, duplicateCount = 0, missingFieldCount = 0;
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string id, name, unit, priceStr, quantityStr;
            std::getline(iss, id, ',');
            std::getline(iss, name, ',');
            std::getline(iss, unit, ',');
            std::getline(iss, priceStr, ',');
            std::getline(iss, quantityStr);

            if (id.empty() || name.empty() || unit.empty() || priceStr.empty() || quantityStr.empty()) {
                std::cout << "Dong du lieu bi thieu du lieu, khong them vao he thong!\n";
                missingFieldCount++;
                continue;
            }
            // Kiểm tra trùng mã sản phẩm
            bool isDuplicate = false;
            for (int i = 0; i < productCount; i++) {
                if (id == products[i].getProductId()) {
                    std::cout << "Ma san pham bi trung: " << id << " (Khong them vao he thong)\n";
                    isDuplicate = true;
                    duplicateCount++;
                    break;
                }
            }
            if (!isDuplicate) {
                if (productCount >= maxProducts) resizeProductArray();
                products[productCount].setProductId(id.c_str());
                products[productCount].setProductName(name.c_str());
                products[productCount].setUnit(unit.c_str());
                products[productCount].setPrice(std::stod(priceStr));
                products[productCount].setQuantity(std::stoi(quantityStr));
                productCount++;
                importedCount++;
            }
        }
        file.close();
        saveProductsToFile();
        std::cout << "Da them " << importedCount << " san pham moi tu file.\n";
        if (duplicateCount > 0) {
            std::cout << duplicateCount << " san pham bi trung ma va khong duoc them.\n";
        }
        if (missingFieldCount > 0) {
            std::cout << missingFieldCount << " san pham bi thieu du lieu va khong duoc them. \n";
        }
        break; 
    }
}
