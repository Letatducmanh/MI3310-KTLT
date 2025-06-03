#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <fstream>
#include <cstring>

class Product {
private:
    char productId[20];
    char productName[100];
    char unit[20];
    double price;
    int quantity; 

public:
    // Constructor
    Product();
    Product(const char* id, const char* name, const char* unitType, double productPrice);
    
    // Destructor
    ~Product();
    
    // Getter methods
    const char* getProductId() const;
    const char* getProductName() const;
    const char* getUnit() const;
    double getPrice() const;
    
    // Setter methods
    void setProductId(const char* id);
    void setProductName(const char* name);
    void setUnit(const char* unitType);
    void setPrice(double productPrice);
    
    // Utility methods
    void displayProduct() const;
    void inputProduct(const Product* existingProducts, int productCount, int ignoreIndex = -1);
    
    // File operations
    void writeToFile(std::ofstream& file) const;
    void readFromFile(std::ifstream& file);
    
    // Operator overloading
    bool operator==(const Product& other) const;
    Product& operator=(const Product& other);

    // Thêm getter/setter cho quantity 
    int getQuantity() const;
    void setQuantity(int qty);
    bool updateQuantity(int qty); // Trả về false nếu không đủ số lượng
};

#endif