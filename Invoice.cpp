#include "Invoice.h"
#include <iomanip>
#include <sstream> 

// Constructor mặc định
Invoice::Invoice() {
    strcpy(invoiceId, "");
    // strcpy(customerName, "");
    strcpy(date, "");
    items = new OrderItem[10];
    itemCount = 0;
    maxItems = 10;
    totalAmount = 0.0;
}

// Constructor có tham số
Invoice::Invoice(const char* id, const char* date) {
    strcpy(invoiceId, id);
    strcpy(this->date, date);
    items = new OrderItem[10];
    itemCount = 0;
    maxItems = 10;
    totalAmount = 0.0;
}

/* Invoice::Invoice(const char* id, const char* customer, const char* date) {
    setInvoiceId(id);
    setCustomerName(customer);
    setDate(date);
    items = new OrderItem[10];
    itemCount = 0;
    maxItems = 10;
    totalAmount = 0.0;
} */ 

// Destructor
Invoice::~Invoice() {
    cleanup();
}

// Copy constructor
Invoice::Invoice(const Invoice& other) {
    copyFrom(other);
}

// Assignment operator
Invoice& Invoice::operator=(const Invoice& other) {
    if (this != &other) {
        cleanup();
        copyFrom(other);
    }
    return *this;
}

// Private methods
void Invoice::copyFrom(const Invoice& other) {
    strcpy(invoiceId, other.invoiceId);
    // strcpy(customerName, other.customerName);
    strcpy(date, other.date);
    itemCount = other.itemCount;
    maxItems = other.maxItems;
    totalAmount = other.totalAmount;
    
    items = new OrderItem[maxItems];
    for (int i = 0; i < itemCount; i++) {
        items[i] = other.items[i];
    }
}

void Invoice::cleanup() {
    delete[] items;
}

const char* Invoice::getCurrentDate() const {
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    static char buffer[20];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);
    return buffer;
}

// Getter methods
const char* Invoice::getInvoiceId() const {
    return invoiceId;
}

/* const char* Invoice::getCustomerName() const {
    return customerName;
} */ 

const char* Invoice::getDate() const {
    return date;
}

int Invoice::getItemCount() const {
    return itemCount;
}

double Invoice::getTotalAmount() const {
    return totalAmount;
}

// Setter methods
void Invoice::setInvoiceId(const char* id) {
    strcpy(invoiceId, id);
}

/* void Invoice::setCustomerName(const char* customer) {
    strcpy(customerName, customer);
} */ 

void Invoice::setCurrentDate() {
    strcpy(date, getCurrentDate());
}

// Item management
void Invoice::addItem(const OrderItem& item) {
    if (itemCount >= maxItems) {
        resizeArray();
    }
    items[itemCount] = item;
    itemCount++;
    calculateTotalAmount();
}

void Invoice::removeItem(int index) {
    if (index >= 0 && index < itemCount) {
        for (int i = index; i < itemCount - 1; i++) {
            items[i] = items[i + 1];
        }
        itemCount--;
        calculateTotalAmount();
    }
}

OrderItem* Invoice::getItems() const {
    return items;
}

void Invoice::resizeArray() {
    int newMaxItems = maxItems * 2;
    OrderItem* newItems = new OrderItem[newMaxItems];
    
    for (int i = 0; i < itemCount; i++) {
        newItems[i] = items[i];
    }
    
    delete[] items;
    items = newItems;
    maxItems = newMaxItems;
}

// Utility methods
void Invoice::calculateTotalAmount() {
    totalAmount = 0.0;
    for (int i = 0; i < itemCount; i++) {
        totalAmount += items[i].getTotalPrice();
    }
}

void Invoice::setDate(const char* date) {
    strcpy(this->date, date);
}
void Invoice::setTotalAmount(double amount) {
    this->totalAmount = amount;
}

void Invoice::clearItems() {
    itemCount = 0;
    // Nếu bạn dùng con trỏ động cho mảng OrderItem, có thể cần delete[] và cấp phát lại nếu cần
}

void Invoice::displayInvoice() const {
    std::cout << "\n=================== HOA DON ===================" << std::endl;
    std::cout << "Ma hoa don: " << invoiceId << std::endl;
    // std::cout << "Khach hang: " << customerName << std::endl;
    std::cout << "Ngay lap: " << date << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << std::left << std::setw(10) << "Ma SP"
              << std::setw(25) << "Ten SP"
              << std::setw(10) << "So luong"
              << std::setw(15) << "Don gia"
              << std::setw(15) << "Thanh tien" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    
    for (int i = 0; i < itemCount; i++) {
        items[i].displayOrderItem();
    }
    
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "TONG TIEN: " << std::fixed << std::setprecision(2) << totalAmount << " VND" << std::endl;
    std::cout << "===============================================\n" << std::endl;
}

void Invoice::printInvoice() const {
    displayInvoice();
}

// File operations
void Invoice::writeToFile(std::ofstream& file) const {
    file.write(invoiceId, sizeof(invoiceId));
    // file.write(customerName, sizeof(customerName));
    file.write(date, sizeof(date));
    file.write((char*)&itemCount, sizeof(itemCount));
    file.write((char*)&totalAmount, sizeof(totalAmount));
    
    for (int i = 0; i < itemCount; i++) {
        items[i].writeToFile(file);
    }
}

void Invoice::readFromFile(std::ifstream& file) {
    file.read(invoiceId, sizeof(invoiceId));
    // file.read(customerName, sizeof(customerName));
    file.read(date, sizeof(date));
    file.read((char*)&itemCount, sizeof(itemCount));
    file.read((char*)&totalAmount, sizeof(totalAmount));
    
    delete[] items;
    maxItems = (itemCount > 10) ? itemCount : 10;
    items = new OrderItem[maxItems];
    
    for (int i = 0; i < itemCount; i++) {
        items[i].readFromFile(file);
    }
}

std::string Invoice::getInvoiceString() const {
    std::ostringstream oss;
    oss << "=================== HOA DON ===================\n";
    oss << "Ma hoa don: " << invoiceId << "\n";
    // oss << "Khach hang: " << customerName << "\n";
    oss << "Ngay lap: " << date << "\n";
    oss << "===============================================\n";
    oss << std::left << std::setw(10) << "Ma SP"
        << std::setw(25) << "Ten SP"
        << std::setw(10) << "So luong"
        << std::setw(15) << "Don gia"
        << std::setw(15) << "Thanh tien" << "\n";
    oss << "-----------------------------------------------\n";
    for (int i = 0; i < itemCount; i++) {
        oss << std::left << std::setw(10) << items[i].getProduct().getProductId()
            << std::setw(25) << items[i].getProduct().getProductName()
            << std::setw(10) << items[i].getQuantity()
            << std::setw(15) << std::fixed << std::setprecision(2) << items[i].getProduct().getPrice()
            << std::setw(15) << std::fixed << std::setprecision(2) << items[i].getTotalPrice() << "\n";
    }
    oss << "-----------------------------------------------\n";
    oss << "TONG TIEN: " << std::fixed << std::setprecision(2) << totalAmount << " VND\n";
    oss << "===============================================\n";
    return oss.str();
}