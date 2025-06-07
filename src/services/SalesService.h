#ifndef SALES_SERVICE_H
#define SALES_SERVICE_H
#include <vector>
#include <string>
#include <map>
#include "models/Product/Product.h"
#include "models/Invoice/Invoice.h"
#include "persistence/DataManager.h"

// Struct dùng cho thống kê
struct ProductRevenue {
    std::string id, name;
    int totalQuantity = 0;
    double totalRevenue = 0.0, priceAtTime = 0.0;
    bool stillExists = false;
};

struct DailyStatistics {
    int invoiceCount = 0;
    double totalRevenue = 0.0;
};

struct MonthlyStatistics {
    int invoiceCount = 0;
    double totalRevenue = 0.0;
};

class SalesService {
private:
    std::vector<Product> products;
    std::vector<Invoice> invoices;
    std::string dataPath;
    DataManager dataManager;
    void loadData();
    void saveData();
public:
    SalesService(const char* exePath);
    ~SalesService();

    // Product Services
    const std::vector<Product>& getAllProducts() const;
    Product* findProductById(const std::string& id);
    const Product* findProductById(const std::string& id) const; 
    bool isProductIdTaken(const std::string& id) const;
    void addProduct(const Product& newProduct);
    bool deleteProduct(const std::string& id);
    ImportResult importProductsFromFile(const std::string& filename);
    const std::string& getDataPath() const;

    // Invoice Services
    const std::vector<Invoice>& getAllInvoices() const;
    Invoice* findInvoiceById(const std::string& id);
    const Invoice* findInvoiceById(const std::string& id) const;
    bool isInvoiceIdTaken(const std::string& id) const;
    void createInvoice(const Invoice& newInvoice);

    // Statistics Services
    DailyStatistics getStatisticsForDate(const std::string& date) const;
    std::map<int, DailyStatistics> getDailyStatisticsForMonth(int month, int year) const;
     std::map<int, MonthlyStatistics> getMonthlyStatisticsForYear(int year) const;
    std::vector<ProductRevenue> getProductRevenueByMonth(int month, int year) const;
};
#endif