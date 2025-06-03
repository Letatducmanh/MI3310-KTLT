#ifndef ORDERITEM_H
#define ORDERITEM_H

#include "Product.h"

class OrderItem {
private:
    Product product;
    int quantity;
    double totalPrice;

public:
    // Constructor
    OrderItem();
    OrderItem(const Product& prod, int qty);
    
    // Destructor  
    ~OrderItem();
    
    // Getter methods
    const Product& getProduct() const;
    int getQuantity() const;
    double getTotalPrice() const;
    
    // Setter methods
    void setProduct(const Product& prod);
    void setQuantity(int qty);
    void calculateTotalPrice();
    
    // Utility methods
    void displayOrderItem() const;
    
    // File operations
    void writeToFile(std::ofstream& file) const;
    void readFromFile(std::ifstream& file);
};

#endif