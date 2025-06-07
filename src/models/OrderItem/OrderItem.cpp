#include "models/OrderItem/OrderItem.h"
#include <utility>

// 'Product p' được truyền vào theo giá trị để tạo một bản sao, "đóng băng" thông tin sản phẩm tại thời điểm giao dịch.
OrderItem::OrderItem(Product p, int qty) : product(std::move(p)), quantity(qty) {
    product.setPrice(this->product.getPrice()); // dòng đảm bảo giá của sản phẩm là giá tại thời điểm mua 
    calculateTotalPrice(); 
}
// hàm tính toán thành tiền của từng sản phẩm 
void OrderItem::calculateTotalPrice() { 
    totalPrice = product.getPrice() * quantity; 
}
// hàm getter : trả về tham chiếu hằng đến đối tượng sản phẩm (là bản sao của thời điểm mua) 
const Product& OrderItem::getProduct() const { 
    return product; 
}
// hàm getter: trả về số lượng của sản phẩm đó 
int OrderItem::getQuantity() const { 
    return quantity; 
}
// hàm getter: trả về thành tiền của mục hàng 
double OrderItem::getTotalPrice() const { 
    return totalPrice; 
}