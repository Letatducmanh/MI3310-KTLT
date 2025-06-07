#ifndef INVOICE_H
#define INVOICE_H
#include <string>
#include <vector>
#include "models/OrderItem/OrderItem.h"

// Khai báo class của đối tượng Invoice (hóa đơn) 
class Invoice {
private:
    std::string invoiceId;
    std::string date;
    std::vector<OrderItem> items;
    double totalAmount;
public:
    Invoice() = default;
    Invoice(const std::string& id, const std::string& date);
    const std::string& getInvoiceId() const;
    const std::string& getDate() const;
    const std::vector<OrderItem>& getItems() const;
    double getTotalAmount() const;
    void addItem(const OrderItem& item);
    void calculateTotalAmount();
};
#endif