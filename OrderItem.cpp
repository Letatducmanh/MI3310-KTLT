#include "OrderItem.h"
#include <iomanip>  

// Constructor mặc định
OrderItem::OrderItem() {
    quantity = 0;
    totalPrice = 0.0;
}

// Constructor có tham số
OrderItem::OrderItem(const Product& prod, int qty) {
    product = prod;
    quantity = qty;
    calculateTotalPrice();
}

// Destructor
OrderItem::~OrderItem() {
    // Không cần làm gì đặc biệt
}

// Getter methods
const Product& OrderItem::getProduct() const {
    return product;
}

int OrderItem::getQuantity() const {
    return quantity;
}

double OrderItem::getTotalPrice() const {
    return totalPrice;
}

// Setter methods
void OrderItem::setProduct(const Product& prod) {
    product = prod;
    calculateTotalPrice();
}

void OrderItem::setQuantity(int qty) {
    quantity = qty;
    calculateTotalPrice();
}

void OrderItem::calculateTotalPrice() {
    totalPrice = product.getPrice() * quantity;
}

// Hiển thị thông tin mục hàng
void OrderItem::displayOrderItem() const {
    std::cout << std::left << std::setw(10) << product.getProductId()
              << std::setw(25) << product.getProductName()
              << std::setw(10) << quantity
              << std::setw(15) << std::fixed << std::setprecision(2) << product.getPrice()
              << std::setw(15) << std::fixed << std::setprecision(2) << totalPrice << std::endl;
}

// Ghi vào file 
void OrderItem::writeToFile(std::ofstream& file) const {
    product.writeToFile(file);
    file.write((char*)&quantity, sizeof(quantity));
    file.write((char*)&totalPrice, sizeof(totalPrice));
}

// Đọc từ file
void OrderItem::readFromFile(std::ifstream& file) {
    product.readFromFile(file);
    file.read((char*)&quantity, sizeof(quantity));
    file.read((char*)&totalPrice, sizeof(totalPrice));
}