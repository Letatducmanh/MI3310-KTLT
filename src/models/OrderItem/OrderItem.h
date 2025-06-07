#ifndef ORDERITEM_H
#define ORDERITEM_H
#include "models/Product/Product.h"

// Khai báo class của đối tượng OrderItem (vật phẩm được đặt hàng) 
class OrderItem {
private:
    Product product;
    int quantity;
    double totalPrice;
    void calculateTotalPrice();
public:
    OrderItem() = default;
    OrderItem(Product p, int qty);
    const Product& getProduct() const;
    int getQuantity() const;
    double getTotalPrice() const;
};
#endif