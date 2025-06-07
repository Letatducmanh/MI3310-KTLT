#include "models/Product/Product.h"
#include <utility>

// hàm dựng : khởi tạo một đối tượng product (sản phẩm ) với đầy đủ thông tin 
Product::Product(std::string id, std::string name, std::string unit, double price, int quantity)
    : productId(std::move(id)), productName(std::move(name)), unit(std::move(unit)), price(price), quantity(quantity) {}
// hàm getter: trả về mã của sản phẩm 
const std::string& Product::getProductId() const { 
    return productId; 
}
// hàm trả về tên của sản phẩm 
const std::string& Product::getProductName() const { 
    return productName; 
}
// hàm trả về đơn vị tính của sản phẩm 
const std::string& Product::getUnit() const { 
    return unit; 
}
// hàm trả về đơn giá của sản phẩm 
double Product::getPrice() const { 
    return price; 
}
// hàm trả về số lượng của sản phẩm 
int Product::getQuantity() const { 
    return quantity; 
}
// hàm cập nhật mã của sản phẩm 
void Product::setProductId(const std::string& id) { 
    productId = id; 
}
// hàm cập nhật tên của sản phẩm 
void Product::setProductName(const std::string& name) { 
    productName = name; 
}
// hàm cập nhật đơn vị của sản phẩm 
void Product::setUnit(const std::string& unitType) { 
    unit = unitType; 
}
// hàm cập nhật đơn giá của sản phẩm 
void Product::setPrice(double productPrice) { 
    price = productPrice; 
}
// hàm cập nhật số lượng của sản phẩm 
void Product::setQuantity(int qty) { 
    quantity = qty; 
}
// hàm thực hiện hành động bán hàng : giảm số lượng tồn kho 
bool Product::sell(int amountToSell) {
    if (quantity >= amountToSell) {
        quantity -= amountToSell;
        return true;
    }
    return false;
}