#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H
#include "services/SalesService.h"

// hàm khai báo class của đối tượng ConsoleUI
class ConsoleUI {
private:
    SalesService& service;
    void handleProductMenu();
    void handleInvoiceMenu();
    void handleStatisticsMenu();
    void displayAllProducts();
    void handleAddProduct();
    void handleEditProduct();
    void handleDeleteProduct();
    void handleSearchProductMenu();
    void handleSearchProductById();
    void handleSearchProductByName();
    void handleImportProducts();
    void displayAllInvoices();
    void handleCreateInvoice();
    void handleDisplayInvoiceById();
    void displayInvoiceDetails(const Invoice& invoice);
    void handleTotalRevenueMenu();
    void handleRevenueByDate();
    void handleRevenueByMonth();
    void handleRevenueByYear();
    void handleProductRevenueByMonth();
    void pressEnterToContinue();
    void clearScreen();
    int getChoice(int min, int max);
    bool isInteger(const std::string& s);
public:
    ConsoleUI(SalesService& service);
    void run();
};
#endif