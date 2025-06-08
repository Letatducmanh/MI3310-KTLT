#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H
#include "services/SalesService.h"

// hàm khai báo class của đối tượng ConsoleUI
class ConsoleUI {
private:
    SalesService& service;
    // Hiển thị các menu con trong chương trình 
    void handleProductMenu();
    void handleInvoiceMenu();
    void handleStatisticsMenu();
    void handleTotalRevenueMenu();
    
    // Các hàm thực hiện chức năng về quản lý sản phẩm trong chương trình
    void handleAddProduct();
    void handleEditProduct();
    void handleDeleteProduct();
    void handleSearchProductMenu();
    void handleSearchProductById();
    void handleSearchProductByName();
    void handleImportProducts();
    void displayAllProducts();
    
    // Các hàm thực hiện chức năng về quản lý hóa đơn trong chương trình
    void displayAllInvoices();
    void handleCreateInvoice();
    void handleDisplayInvoiceById();
    void displayInvoiceDetails(const Invoice& invoice);
    
    // Các hàm thực hiện chức năng về báo cáo thống kê trong chương trình 
    void handleRevenueByDate();
    void handleRevenueByMonth();
    void handleRevenueByYear();
    void handleProductRevenueByMonth();
    
    // Các hàm hỗ trợ 
    void pressEnterToContinue();
    void clearScreen();
    int getChoice(int min, int max);
    bool isInteger(const std::string& s);
    bool isValidDate(int day, int month, int year);
public:
    ConsoleUI(SalesService& service);
    void run();
};
#endif