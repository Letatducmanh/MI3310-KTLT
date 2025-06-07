#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>

// khai báo class của đối tượng Product (đồ vật/ vật phẩm) 
class Product {
private:
    std::string productId;
    std::string productName;
    std::string unit;
    double price;
    int quantity;
public:
    Product() = default;
    Product(std::string id, std::string name, std::string unit, double price, int quantity);
    const std::string& getProductId() const;
    const std::string& getProductName() const;
    const std::string& getUnit() const;
    double getPrice() const;
    int getQuantity() const;
    void setProductId(const std::string& id); 
    void setProductName(const std::string& name);
    void setUnit(const std::string& unitType);
    void setPrice(double productPrice);
    void setQuantity(int qty);
    bool sell(int amountToSell);
};
#endif