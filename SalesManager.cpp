#include "SalesManager.h"
#include <iomanip>
#include <sstream>
#include <limits> 
#include <algorithm>

// Constructor
SalesManager::SalesManager() {
    products = new Product[10];
    productCount = 0;
    maxProducts = 10;
    
    invoices = new Invoice[10];
    invoiceCount = 0;
    maxInvoices = 10;
}

// Destructor
SalesManager::~SalesManager() {
    cleanup();
}

void SalesManager::cleanup() {
    delete[] products;
    delete[] invoices;
}

// Array resizing
void SalesManager::resizeProductArray() {
    int newMaxProducts = maxProducts * 2;
    Product* newProducts = new Product[newMaxProducts];
    
    for (int i = 0; i < productCount; i++) {
        newProducts[i] = products[i];
    }
    
    delete[] products;
    products = newProducts;
    maxProducts = newMaxProducts;
}

// Thay thế toàn bộ hàm resizeInvoiceArray() bằng:
void SalesManager::resizeInvoiceArray() {
    int newMaxInvoices = maxInvoices * 2;
    Invoice* newInvoices = new Invoice[newMaxInvoices];

    for (int i = 0; i < invoiceCount; i++) {
        newInvoices[i] = invoices[i];
    }

    delete[] invoices;
    invoices = newInvoices;
    maxInvoices = newMaxInvoices;
}

// Utility methods
int SalesManager::generateInvoiceNumber() const {
    return invoiceCount + 1;
}

bool SalesManager::isValidDate(const char* dateStr) const {
    // Kiểm tra độ dài
    if (strlen(dateStr) != 10) return false;
    // Kiểm tra vị trí dấu '/'
    if (dateStr[2] != '/' || dateStr[5] != '/') return false;
    // Kiểm tra các ký tự còn lại là số
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(dateStr[i])) return false;
    }
    return true;
}

void SalesManager::clearInputBuffer() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}



    