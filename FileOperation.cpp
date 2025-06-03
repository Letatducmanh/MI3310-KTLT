#include "SalesManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

// File operations
void SalesManager::saveProductsToFile() const {
    std::ofstream file("products.txt", std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Khong the mo file de ghi san pham!" << std::endl;
        return;
    }
    
    for (int i = 0; i < productCount; i++) {
        file << products[i].getProductId() << ","
             << products[i].getProductName() << ","
             << products[i].getUnit() << ","
             << products[i].getPrice() << ","
             << products[i].getQuantity() << std::endl;
    }
    file.close();
}

void SalesManager::loadProductsFromFile() {
    std::ifstream file("products.txt");
    if (!file.is_open()) {
        return; // File không tồn tại, bỏ qua
    }

    productCount = 0;
    std::string line;
    while (std::getline(file, line) && productCount < maxProducts) {
        std::istringstream iss(line);
        std::string id, name, unit, priceStr, quantityStr;
        std::getline(iss, id, ',');
        std::getline(iss, name, ',');
        std::getline(iss, unit, ',');
        std::getline(iss, priceStr, ',');
        std::getline(iss, quantityStr);

        if (!id.empty() && !name.empty() && !unit.empty() && !priceStr.empty() && !quantityStr.empty()) {
            products[productCount].setProductId(id.c_str());
            products[productCount].setProductName(name.c_str());
            products[productCount].setUnit(unit.c_str());
            products[productCount].setPrice(std::stod(priceStr));
            products[productCount].setQuantity(std::stoi(quantityStr)); // Đảm bảo đọc số lượng
            productCount++;
        }
    }
    file.close();
}
void SalesManager::saveInvoicesToFile() const {
    std::ofstream file("invoices.txt");
    if (!file.is_open()) {
        std::cout << "Khong the mo file de ghi hoa don!" << std::endl;
        return;
    }

    for (int i = 0; i < invoiceCount; i++) {
        file << invoices[i].getInvoiceId() << ","
             // << invoices[i].getCustomerName() << ","
             << invoices[i].getDate() << ","
             << invoices[i].getTotalAmount() << std::endl;
    }
    file.close();
}

void SalesManager::loadInvoicesFromFile() {
    std::ifstream file("invoices.txt");
    if (!file.is_open()) {
        return; // File không tồn tại, bỏ qua
    }

    invoiceCount = 0;
    std::string line;
    while (std::getline(file, line)) {
        // Nếu đã đầy mảng thì tăng kích thước 
        if (invoiceCount >= maxInvoices) {
            resizeInvoiceArray(); 
        }
        std::istringstream iss(line);
        std::string id, customer, date, totalStr;

        std::getline(iss, id, ',');
        // std::getline(iss, customer, ',');
        std::getline(iss, date, ',');
        std::getline(iss, totalStr);

        if (!id.empty() && !date.empty() && !totalStr.empty()) {
            invoices[invoiceCount].setInvoiceId(id.c_str());
            // invoices[invoiceCount].setCustomerName(customer.c_str());
            invoices[invoiceCount].setDate(date.c_str());
            invoices[invoiceCount].setTotalAmount(std::stod(totalStr));
            invoiceCount++;
        }
    }
    file.close();
}

void SalesManager::saveOrdersToFile() const {
    std::ofstream file("orders.txt");
    if (!file.is_open()) {
        std::cout << "Khong the mo file de ghi don hang!" << std::endl;
        return;
    }

    for (int i = 0; i < invoiceCount; i++) {
        file << "INVOICE," << invoices[i].getInvoiceId() << "," << invoices[i].getDate() << std::endl;
        OrderItem* items = invoices[i].getItems();
        int itemCount = invoices[i].getItemCount();
        for (int j = 0; j < itemCount; j++) {
            file << "ITEM," << items[j].getProduct().getProductId() << ","
                 << items[j].getProduct().getProductName() << ","
                 << items[j].getQuantity() << ","
                 << items[j].getTotalPrice() << std::endl;
        }
        file << "END" << std::endl; // Đánh dấu kết thúc hóa đơn
    }
    file.close();
}

void SalesManager::loadOrdersFromFile() {
    std::ifstream file("orders.txt");
    if (!file.is_open()) {
        return;
    }

    std::string line;
    Invoice* currentInvoice = nullptr;
    // int currentInvoiceIndex = -1;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tag;
        std::getline(iss, tag, ',');

        if (tag == "INVOICE") {
            std::string id, date;
            std::getline(iss, id, ',');
            std::getline(iss, date);

            // Tìm hóa đơn theo id và ngày
            currentInvoice = nullptr;
            for (int i = 0; i < invoiceCount; i++) {
                if (strcmp(invoices[i].getInvoiceId(), id.c_str()) == 0 &&
                    strcmp(invoices[i].getDate(), date.c_str()) == 0) {
                    currentInvoice = &invoices[i];
                    // XÓA DANH SÁCH SẢN PHẨM CŨ TRƯỚC KHI NẠP MỚI
                    currentInvoice->clearItems(); // <-- Thêm dòng này
                    break;
                }
            }
        } else if (tag == "ITEM" && currentInvoice != nullptr) {
            std::string pid, pname, qtyStr, priceStr;
            std::getline(iss, pid, ',');
            std::getline(iss, pname, ',');
            std::getline(iss, qtyStr, ',');
            std::getline(iss, priceStr);

            Product* prod = findProductById(pid.c_str());
            if (prod) {
                OrderItem item(*prod, std::stoi(qtyStr));
                currentInvoice->addItem(item);
            } else {
                // Nếu không tìm thấy sản phẩm, tạo tạm thời
                Product temp(pid.c_str(), pname.c_str(), "", 0.0);
                OrderItem item(temp, std::stoi(qtyStr));
                currentInvoice->addItem(item);
            }
        } else if (tag == "END") {
            currentInvoice = nullptr;
            // currentInvoiceIndex = -1;
        }
    }
    file.close();
}


void SalesManager::saveAllData() const {
    saveProductsToFile();
    saveInvoicesToFile();
    saveOrdersToFile();
}

void SalesManager::loadAllData() {
    loadProductsFromFile();
    loadInvoicesFromFile();
    loadOrdersFromFile();
}
