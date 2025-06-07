#ifndef SALESMANAGER_H
#define SALESMANAGER_H

#include "Invoice.h"

class SalesManager {
private:
    Product* products;
    int productCount;
    int maxProducts;
    
    Invoice* invoices;
    int invoiceCount;
    int maxInvoices;

    void resizeProductArray();
    void resizeInvoiceArray();
    void cleanup();
    int generateInvoiceNumber() const;
    bool isValidDate(const char* dateStr) const;

public:
    // Constructor
    SalesManager();
    
    // Destructor
    ~SalesManager();
    
    // Product management
    void addProduct();
    void editProduct();
    void deleteProduct();
    void displayAllProducts() const;
    void searchProductById() const;
    void searchProductByName() const;
    void importProductsFromFile(const char* filename);  
    Product* findProductById(const char* id) const ;
    
    // Invoice management
    void createInvoice();
    void displayAllInvoices() const;
    void displayInvoiceById(const char* id) const;
    Invoice* findInvoiceById(const char* id);
    
    // Statistics
    void displayRevenueByDate() const;
    void displayRevenueByMonth() const;
    double calculateTotalRevenue(int month, int year) const;
    void calculateTotalRevenueByMonthMenu() const;
    void calculateTotalRevenueByYearMenu() const;
    
    // File operations
    void saveProductsToFile() const;
    void loadProductsFromFile();
    void saveInvoicesToFile() const;
    void loadInvoicesFromFile();
    void saveAllData() const;
    void loadAllData();
    void saveOrdersToFile() const;
    void loadOrdersFromFile();

    void clearInputBuffer() const;

};

#endif
