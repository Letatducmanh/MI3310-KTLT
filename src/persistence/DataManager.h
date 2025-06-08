#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include <string>
#include <vector>
#include <utility> 
#include "models/Product/Product.h"
#include "models/Invoice/Invoice.h"

// cấu trúc ImportResult dùng để tóm tắt kết quả của 1 hành động import file
struct ImportResult {
    int importedCount = 0;
    int duplicateCount = 0;
    int missingFieldCount = 0;
};

// khai báo class về đối tượng DataManager 
class DataManager {
public:
    void loadAllData(std::vector<Product>& products, std::vector<Invoice>& invoices) ;
    void saveAllData(const std::vector<Product>& products, const std::vector<Invoice>& invoices) const ;
    void appendInvoiceToLog(const Invoice& invoice) const ;
    DataManager(const std::string& dataDirectory);
    std::pair<std::vector<Product>, ImportResult> loadProductsFromImportFile(const std::string& filename, const std::vector<Product>& existingProducts);
private:
    const std::string productsFile;
    const std::string invoicesFile ;
    const std::string ordersFile ;
    const std::string invoiceLogFile ;

    // Các hàm private helper
    void loadProducts(std::vector<Product>& products);
    void saveProducts(const std::vector<Product>& products) const;
    void loadInvoices(std::vector<Invoice>& invoices) ;
    void saveInvoices(const std::vector<Invoice>& invoices) const; 
    void loadOrderItems(std::vector<Invoice>& invoices, const std::vector<Product>& allProducts);
    void saveOrderItems(const std::vector<Invoice>& invoices) const;
    std::string getInvoiceStringForLog(const Invoice& invoice) const;
};
#endif