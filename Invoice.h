#ifndef INVOICE_H
#define INVOICE_H

#include "OrderItem.h"
#include <ctime>

class Invoice {
private:
    char invoiceId[20];
    // char customerName[100];
    char date[20];
    OrderItem* items;
    int itemCount;
    int maxItems;
    double totalAmount;

public:
    // Constructor
    Invoice();
    // Invoice(const char* id, const char* customer);
    Invoice(const char* id, const char* date); 
    // Destructor
    ~Invoice();
    
    // Copy constructor
    Invoice(const Invoice& other);
    
    // Assignment operator
    Invoice& operator=(const Invoice& other);
    
    // Getter methods
    const char* getInvoiceId() const;
    // const char* getCustomerName() const;
    const char* getDate() const;
    int getItemCount() const;
    double getTotalAmount() const;
    
    // Setter methods
    void setInvoiceId(const char* id);
    // void setCustomerName(const char* customer);
    void setCurrentDate();
    
    // Item management
    void addItem(const OrderItem& item);
    void removeItem(int index);
    OrderItem* getItems() const;
    
    // Utility methods
    void calculateTotalAmount();
    void displayInvoice() const;
    void printInvoice() const;
    
    // File operations
    void writeToFile(std::ofstream& file) const;
    void readFromFile(std::ifstream& file);

    void setDate(const char* date);
    void setTotalAmount(double amount);

    std::string getInvoiceString() const;

    void clearItems();

private:
    void resizeArray();
    void copyFrom(const Invoice& other);
    void cleanup();
    const char* getCurrentDate() const;
};

#endif