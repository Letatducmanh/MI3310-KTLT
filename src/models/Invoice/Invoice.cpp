#include "models/Invoice/Invoice.h"

// Hàm dựng: Khởi tạo một hóa đơn mới với mã và ngày, tổng tiền ban đầu là 0.
Invoice::Invoice(const std::string& id, const std::string& date)
    : invoiceId(id), date(date), totalAmount(0.0) {}
// Hàm ttrả về mã của hóa đơn.
const std::string& Invoice::getInvoiceId() const { 
    return invoiceId; 
}
// Hàm trả về ngày lập của hóa đơn.
const std::string& Invoice::getDate() const { 
    return date; 
}
// Hàm trả về một tham chiếu hằng đến danh sách các sản phẩm trong hóa đơn.
const std::vector<OrderItem>& Invoice::getItems() const { 
    return items; 
}
// Hàm trả về tổng số tiền của hóa đơn.
double Invoice::getTotalAmount() const { 
    return totalAmount; 
}
// Hàm thêm 1 sản phẩm (mục hàng) vào hóa đơn và tự động tính toán lại tổng tiền 
void Invoice::addItem(const OrderItem& item) {
    items.push_back(item);
    calculateTotalAmount();
}
// Hàm tính toán tổng lại số tiền của hóa đơn bằng cách cộng dồn giá trị của từng mục hàng
void Invoice::calculateTotalAmount() {
    totalAmount = 0.0;
    for (const auto& item : items) {
        totalAmount += item.getTotalPrice();
    }
}