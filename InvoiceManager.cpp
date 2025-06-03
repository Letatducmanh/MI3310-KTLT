#include "SalesManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits> 
#include <algorithm>

// Invoice management
void SalesManager::createInvoice() {
    std::cout << "Cac ma hoa don da co: ";
    for (int i = 0; i < invoiceCount; i++) {
        std::cout << invoices[i].getInvoiceId() << " ";
    }
    std::cout << std::endl;

    if (invoiceCount >= maxInvoices) {
        resizeInvoiceArray();
    }

    int invoiceNumber;
    char invoiceId[20];

    // Nhập mã hóa đơn là số nguyên
    do {
        std::cout << "\n=== TAO HOA DON MOI ===" << std::endl;
        std::cout << "Nhap ma hoa don (so nguyen): ";
        std::cin >> invoiceNumber;
        if (std::cin.fail() || invoiceNumber <= 0) {
            std::cout << "Ma hoa don phai la so nguyen duong! Vui long nhap lai.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        // Kiểm tra trùng mã hóa đơn
        bool existed = false;
        for (int i = 0; i < invoiceCount; i++) {
            if (atoi(invoices[i].getInvoiceId()) == invoiceNumber) {
                existed = true;
                break;
            }
        }
        if (existed) {
            std::cout << "Ma hoa don da ton tai! Vui long nhap ma khac.\n";
            continue;
        }
        break;
    } while (true);

    // Chuyển số nguyên thành chuỗi để lưu vào Invoice
    sprintf(invoiceId, "%d", invoiceNumber);

    // Nhập ngày lập hóa đơn
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm
    char date[20];
    do {
        std::cout << "Nhap ngay lap hoa don (dd/mm/yyyy): ";
        std::cin.getline(date, 20);

        // Kiểm tra định dạng: độ dài 10, ký tự thứ 3 và 6 là '/'
        if (strlen(date) == 10 && date[2] == '/' && date[5] == '/') {
            // Có thể kiểm tra thêm ngày, tháng, năm là số nếu muốn
            break;
        } else {
            std::cout << "Dinh dang ngay khong hop le! Vui long nhap lai (dd/mm/yyyy)." << std::endl;
        }
    } while (true);
    // Sử dụng tên khách hàng mặc định
    // const char* defaultCustomerName = "Khach hang vang lai";

    // Khởi tạo hóa đơn với mã hóa đơn, tên khách hàng và ngày nhập
    invoices[invoiceCount] = Invoice(invoiceId, date);

    char choice;
    do {
        displayAllProducts();

        char productId[20];
        int quantity;

        // Vòng lặp nhập mã sản phẩm
        bool foundProduct = false;
        do {
            std::cout << "Nhap ma san pham: ";
            std::cin >> productId;

            Product* product = findProductById(productId);
            if (product == nullptr) {
                std::cout << "Ma san pham khong ton tai!" << std::endl;
                std::cout << "Ban co muon tiep tuc nhap ma san pham khac? (y/n): ";
                char retry;
                std::cin >> retry;
                if (retry == 'n' || retry == 'N') {
                    return; // Thoát về menu chính
                }
                // Nếu chọn y thì tiếp tục nhập lại mã sản phẩm
            } 
            else {
                foundProduct = true;
                do {
                    std::cout << "Nhap so luong: ";
                    std::cin >> quantity;

                    if (quantity <= 0) {
                        std::cout << "So luong phai lon hon 0!" << std::endl;
                        continue;
                    }
                    if (product->getQuantity() < quantity) {
                        std::cout << "San pham chi con " << product->getQuantity() << " "
                                << product->getUnit() << "!" << std::endl;
                        std::cout << "Ban co muon tiep tuc nhap so luong cho san pham nay? (y/n): ";
                        char retryQty;
                        std::cin >> retryQty;
                        if (retryQty == 'n' || retryQty == 'N') {
                            foundProduct = false; // Quay lại nhập sản phẩm khác
                            break;
                        } else {
                            continue; // Nhập lại số lượng cho sản phẩm này
                        }
                    }

                    if (product->updateQuantity(quantity)) {
                        OrderItem item(*product, quantity);
                        invoices[invoiceCount].addItem(item);
                        std::cout << "Them san pham vao hoa don thanh cong!" << std::endl;
                        saveProductsToFile();
                    }
                    break; // Thoát khỏi vòng lặp nhập số lượng
                } while (true);
            }
        } while (!foundProduct);

        std::cout << "Ban co muon them san pham khac? (y/n): ";
        std::cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    invoices[invoiceCount].printInvoice();
    invoiceCount++;
    std::cout << "Hoa don da duoc tao thanh cong! " << std::endl;
    saveInvoicesToFile();
    
    std::ofstream outFile("invoices1.txt", std::ios::app); // ghi nối tiếp
    outFile << invoices[invoiceCount -1 ].getInvoiceString() << std::endl;
    outFile.close();
    saveOrdersToFile();
}

void SalesManager::displayAllInvoices() const {
    if (invoiceCount == 0) {
        std::cout << "\nKhong co hoa don nao trong he thong!" << std::endl;
        return;
    }
    
    std::cout << "\n=== DANH SACH HOA DON ===" << std::endl;
    std::cout << std::left << std::setw(15) << "Ma hoa don"
              // << std::setw(25) << "Khach hang"
              << std::setw(15) << "Ngay lap"
              << std::setw(15) << "Tong tien" << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    
    for (int i = 0; i < invoiceCount; i++) {
        std::cout << std::left << std::setw(15) << invoices[i].getInvoiceId()
                  // << std::setw(25) << invoices[i].getCustomerName()
                  << std::setw(15) << invoices[i].getDate()
                  << std::setw(15) << std::fixed << std::setprecision(2) 
                  << invoices[i].getTotalAmount() << std::endl;
    }
    std::cout << std::endl;
}

void SalesManager::displayInvoiceById(const char* id) const {
    char inputId[20];
    strcpy(inputId, id);

    while (true) {
        Invoice* invoice = const_cast<SalesManager*>(this)->findInvoiceById(inputId);
        if (invoice != nullptr) {
            invoice->displayInvoice();
            break;
        } else {
            std::cout << "Khong tim thay ma hoa don. Ban co muon tiep tuc su dung chuc nang nay khong? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                std::cout << "Nhap lai ma hoa don: ";
                std::cin >> inputId;
            } else {
                std::cout << "Quay ve man hinh chinh.\n";
                break;
            }
        }
    }
}

Invoice* SalesManager::findInvoiceById(const char* id) {
    for (int i = 0; i < invoiceCount; i++) {
        if (strcmp(invoices[i].getInvoiceId(), id) == 0) {
            return &invoices[i];
        }
    }
    return nullptr;
}
