#include "Product.h"
#include <iomanip>

// Constructor mặc định
Product::Product() {
    strcpy(productId, "");
    strcpy(productName, "");
    strcpy(unit, "");
    price = 0.0;
    quantity = 0; 
}

// Constructor có tham số
Product::Product(const char* id, const char* name, const char* unitType, double productPrice) {
    strcpy(productId, id);
    strcpy(productName, name);
    strcpy(unit, unitType);
    price = productPrice;
    quantity = 0;
}

// Destructor
Product::~Product() {
    // Không cần làm gì đặc biệt vì không sử dụng dynamic memory
}

// Thêm getter/setter cho quantity 
int Product::getQuantity() const {
    return quantity;
}

void Product::setQuantity(int qty) {
    if (qty >= 0) {
        quantity = qty;
    }
}

bool Product::updateQuantity(int qty) {
    if (quantity >= qty) {
        quantity -= qty;
        return true;
    }
    return false;
}


// Getter methods
const char* Product::getProductId() const {
    return productId;
}

const char* Product::getProductName() const {
    return productName;
}

const char* Product::getUnit() const {
    return unit;
}

double Product::getPrice() const {
    return price;
}

// Setter methods
void Product::setProductId(const char* id) {
    strcpy(productId, id);
}

void Product::setProductName(const char* name) {
    if (name != nullptr) {
        strcpy(this->productName, name); // Sao chép giá trị vào biến thành viên
    }
}

void Product::setUnit(const char* unitType) {
    strcpy(unit, unitType);
}

void Product::setPrice(double productPrice) {
    price = productPrice;
}

// Hiển thị thông tin sản phẩm
void Product::displayProduct() const {
    std::cout << std::left << std::setw(10) << productId 
              << std::setw(25) << productName 
              << std::setw(10) << unit 
              << std::setw(15) << std::fixed << std::setprecision(2) << price 
              << std::setw(15) << quantity << std::endl;
}

// Nhập thông tin sản phẩm
void Product::inputProduct(const Product* existingProducts, int productCount, int ignoreIndex) {
    bool isValid = false;

    // Nhập mã sản phẩm
    do {
        std::cout << "Nhap ma san pham (so nguyen): ";
        std::cin >> productId;

        // Kiểm tra mã sản phẩm chỉ chứa số nguyên
        bool isInteger = true;
        for (int i = 0; productId[i] != '\0'; i++) {
            if (!isdigit(productId[i])) {
                isInteger = false;
                break;
            }
        }

        if (!isInteger) {
            std::cout << "Ma san pham chi duoc chua so nguyen! Vui long nhap lai.\n";
            continue;
        }

        // Kiểm tra mã sản phẩm có bị trùng không
        bool isDuplicate = false;
        for (int i = 0; i < productCount; i++) {
            if (i == ignoreIndex) continue; // Bỏ qua chính nó
            if (strcmp(existingProducts[i].getProductId(), productId) == 0) {
                isDuplicate = true;
                break;
            }
        }

        if (isDuplicate) {
            std::cout << "Ma san pham bi trung! Vui long nhap lai.\n";
            continue;
        }

        isValid = true; // Mã sản phẩm hợp lệ
    } while (!isValid);

    std::cin.ignore(); // Xóa bộ đệm

    // Nhập các thông tin khác
    std::cout << "Nhap ten san pham: ";
    std::cin.getline(productName, sizeof(productName));

    std::cout << "Nhap don vi tinh: ";
    std::cin >> unit;

    std::cout << "Nhap don gia: ";
    std::cin >> price;

    std::cout << "Nhap so luong: ";
    std::cin >> quantity;
    if (quantity < 0) {
        quantity = 0;
    }
}


// Ghi vào file
void Product::writeToFile(std::ofstream& file) const {
    file.write((char*)&productId, sizeof(productId));
    file.write(productName, sizeof(productName));
    file.write(unit, sizeof(unit));
    file.write((char*)&price, sizeof(price));
    file.write((char*)&quantity, sizeof(quantity));
}

// Đọc từ file
void Product::readFromFile(std::ifstream& file) {
    file.read((char*)&productId, sizeof(productId));
    file.read(productName, sizeof(productName));
    file.read(unit, sizeof(unit));
    file.read((char*)&price, sizeof(price));
    file.read((char*)&quantity, sizeof(quantity));
}

// Operator ==
bool Product::operator==(const Product& other) const {
    return strcmp(productId, other.productId) == 0;
}

// Operator =
Product& Product::operator=(const Product& other) {
    if (this != &other) {
        strcpy(productId, other.productId);
        strcpy(productName, other.productName);
        strcpy(unit, other.unit);
        price = other.price;
    }
    return *this;
}