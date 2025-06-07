#include "ui/Console/ConsoleUI.h"
#include "ui/Menu/Menu.h"
#include "log/Logger.h" 
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <filesystem> 

ConsoleUI::ConsoleUI(SalesService& service) : service(service) {}
void ConsoleUI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void ConsoleUI::pressEnterToContinue() {
    std::cout << "\nNhan Enter de tiep tuc...";
    std::cin.get();
}
int ConsoleUI::getChoice(int min, int max) {
    int choice;
    std::cout << "Lua chon cua ban: ";
    while (!(std::cin >> choice) || choice < min || choice > max) {
        std::cout << "Lua chon khong hop le! Vui long nhap lai (" << min << "-" << max << "): ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}
bool ConsoleUI::isInteger(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool ConsoleUI::isValidDate(int day, int month, int year) {
    // Kiểm tra năm, tháng, ngày cơ bản
    if (year < 1000 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }

    // Kiểm tra các tháng có 30 ngày
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return false;
    }

    // Kiểm tra riêng cho tháng 2 (năm nhuận)
    if (month == 2) {
        // Quy tắc năm nhuận: chia hết cho 4 VÀ (không chia hết cho 100 HOẶC chia hết cho 400)
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap) {
            return day <= 29; // Năm nhuận, tháng 2 có 29 ngày
        } else {
            return day <= 28; // Năm không nhuận, tháng 2 có 28 ngày
        }
    }

    // Nếu qua được tất cả các kiểm tra, ngày là hợp lệ
    return true;
}

void ConsoleUI::run() {
    int choice;
    do {
        clearScreen();
        displayMainMenu();
        choice = getChoice(0, 3);
        Logger::getInstance().log("Menu Chinh: Nguoi dung chon chuc nang " + std::to_string(choice));
        switch (choice) {
            case 1: handleProductMenu(); break;
            case 2: handleInvoiceMenu(); break;
            case 3: handleStatisticsMenu(); break;
            case 0: std::cout << "Dang luu du lieu... Cam on ban da su dung chuong trinh!\n"; break;
        }
    } while (choice != 0);
}

// --- PRODUCT UI ---
void ConsoleUI::handleProductMenu() {
    Logger::getInstance().log("Truy cap Menu Quan Ly San Pham.");
    int choice;
    do {
        clearScreen();
        displayProductMenu();
        choice = getChoice(0, 6);
        Logger::getInstance().log("Menu San Pham: Nguoi dung chon chuc nang " + std::to_string(choice));
        switch (choice) {
            case 1: handleAddProduct(); break;
            case 2: handleEditProduct(); break;
            case 3: handleDeleteProduct(); break;
            case 4: displayAllProducts(); break;
            case 5: handleSearchProductMenu(); break;
            case 6: handleImportProducts(); break;
        }
        if (choice != 0) pressEnterToContinue();
    } while (choice != 0);
}

void ConsoleUI::handleSearchProductMenu() {
    int choice;
    do {
        clearScreen();
        displaySearchProductMenu();
        choice = getChoice(0, 2);
        switch (choice) {
            case 1: handleSearchProductById(); break;
            case 2: handleSearchProductByName(); break;
        }
        if (choice != 0) pressEnterToContinue();
    } while(choice != 0);
}


void ConsoleUI::displayAllProducts() {
    clearScreen();
    const auto& products = service.getAllProducts();
    std::cout << "\n=== DANH SACH SAN PHAM ===" << std::endl;
    if (products.empty()) {
        std::cout << "Chua co san pham nao trong he thong." << std::endl;
        return;
    }
    std::cout << std::left << std::setw(10) << "Ma SP" << std::setw(25) << "Ten SP" << std::setw(10) << "Don vi"
              << std::setw(15) << "Don gia" << std::setw(10) << "So luong" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    for (const auto& p : products) {
        std::cout << std::left << std::setw(10) << p.getProductId() << std::setw(25) << p.getProductName()
                  << std::setw(10) << p.getUnit() << std::setw(15) << std::fixed << std::setprecision(0) << p.getPrice()
                  << std::setw(10) << p.getQuantity() << std::endl;
    }
}

void ConsoleUI::handleAddProduct() {
    clearScreen();
    std::cout << "=== THEM SAN PHAM MOI ===" << std::endl;
    std::string id, name, unit; double price; int quantity;
    do {
        std::cout << "Nhap ma san pham (so nguyen): ";
        std::getline(std::cin, id);
        if(!isInteger(id)) { std::cout << "Ma san pham phai la so nguyen.\n"; continue; }
        if (service.isProductIdTaken(id)) { std::cout << "Ma san pham da ton tai.\n"; }
    } while (!isInteger(id) || service.isProductIdTaken(id));
    std::cout << "Nhap ten san pham: "; std::getline(std::cin, name);
    std::cout << "Nhap don vi tinh: "; std::getline(std::cin, unit);
    std::cout << "Nhap don gia: ";
    while (!(std::cin >> price) || price < 0) {
        std::cout << "Don gia khong hop le (phai la so duong). Vui long nhap lai: ";
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "Nhap so luong: ";
    while (!(std::cin >> quantity) || quantity < 0) {
        std::cout << "So luong khong hop le (phai la so nguyen duong). Vui long nhap lai: ";
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    service.addProduct(Product(id, name, unit, price, quantity));
    std::string logMessage = "Them san pham thanh cong. ID: " + id + ", Ten: " + name;
    Logger::getInstance().log(logMessage);
    std::cout << "Them san pham thanh cong!" << std::endl;
}

void ConsoleUI::handleEditProduct() {
    clearScreen();
    displayAllProducts();
    if (service.getAllProducts().empty()) {
        return; // Không có gì để sửa
    }

    std::cout << "\nNhap ma san pham can sua: ";
    std::string oldId;
    std::getline(std::cin, oldId);

    Product* p = service.findProductById(oldId);
    if (!p) {
        Logger::getInstance().log("Sua san pham THAT BAI. Khong tim thay ID: " + oldId);
        std::cout << "Khong tim thay san pham voi ma: " << oldId << std::endl;
        return;
    }

    std::cout << "Tim thay san pham: " << p->getProductName() << ". Nhap thong tin moi (nhan Enter de bo qua):" << std::endl;
    
    Product temp = *p; // Tạo một bản sao để chỉnh sửa
    std::string input;

    do {
        std::cout << "Ma san pham moi (" << temp.getProductId() << "): ";
        std::getline(std::cin, input);
        if (input.empty()) {
            break; // Người dùng bỏ qua, giữ lại mã cũ
        }
        if (!isInteger(input)) {
            std::cout << "Loi: Ma san pham phai la so nguyen.\n";
            continue;
        }
        // Kiểm tra xem mã mới có bị trùng với một sản phẩm KHÁC không
        if (service.isProductIdTaken(input) && input != oldId) {
            std::cout << "Loi: Ma san pham '" << input << "' da ton tai.\n";
            continue;
        }
        temp.setProductId(input);
        break; // Mã hợp lệ, thoát vòng lặp
    } while (true);
    // -----------------------------------------

    std::cout << "Ten san pham (" << temp.getProductName() << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) temp.setProductName(input);

    std::cout << "Don vi (" << temp.getUnit() << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) temp.setUnit(input);

    std::cout << "Don gia (" << temp.getPrice() << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        try {
            double newPrice = std::stod(input);
            if (newPrice >= 0) temp.setPrice(newPrice);
            else std::cout << "Gia phai la so duong. Giu nguyen gia cu.\n";
        } catch (...) { std::cout << "Don gia khong hop le, giu nguyen gia cu.\n"; }
    }

    std::cout << "So luong (" << temp.getQuantity() << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        try {
            int newQuantity = std::stoi(input);
            if (newQuantity >= 0) temp.setQuantity(newQuantity);
            else std::cout << "So luong phai la so nguyen duong. Giu nguyen so luong cu.\n";
        } catch (...) { std::cout << "So luong khong hop le, giu nguyen so luong cu.\n"; }
    }
    
    // Gán đối tượng đã chỉnh sửa ngược lại vào sản phẩm gốc
    *p = temp; 
    Logger::getInstance().log("Sua san pham thanh cong. ID: " + oldId);
    std::cout << "Cap nhat san pham thanh cong!" << std::endl;
}

void ConsoleUI::handleDeleteProduct() {
    char choice;
    do {
        clearScreen(); displayAllProducts();
        std::cout << "\nNhap ma san pham can xoa: ";
        std::string id; std::getline(std::cin, id);
        if (service.deleteProduct(id)) {
            Logger::getInstance().log("Xoa san pham thanh cong. ID: " + id);
            std::cout << "Xoa san pham thanh cong!" << std::endl;
            break;
        } else {
            Logger::getInstance().log("Xoa san pham THAT BAI. Khong tim thay ID: " + id);
            std::cout << "Khong tim thay san pham voi ma: " << id << std::endl;
            std::cout << "Ban co muon thu lai khong? (y/n): ";
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice != 'y' && choice != 'Y') break;
        }
    } while(true);
}

void ConsoleUI::handleSearchProductById() {
    char choice;
    do {
        clearScreen();
        std::cout << "Nhap ma san pham can tim: ";
        std::string id; std::getline(std::cin, id);
        Product* p = service.findProductById(id);
        if (p) {
            std::cout << "\n=== KET QUA TIM KIEM ===" << std::endl;
            std::cout << std::left << std::setw(10) << "Ma SP" << std::setw(25) << "Ten SP" << std::setw(10) << "Don vi"
                      << std::setw(15) << "Don gia" << std::setw(10) << "So luong" << std::endl;
            std::cout << std::string(70, '-') << std::endl;
            std::cout << std::left << std::setw(10) << p->getProductId() << std::setw(25) << p->getProductName()
                      << std::setw(10) << p->getUnit() << std::setw(15) << std::fixed << std::setprecision(0) << p->getPrice()
                      << std::setw(10) << p->getQuantity() << std::endl;
            break;
        } else {
            std::cout << "Khong tim thay san pham nao voi ma '" << id << "'." << std::endl;
            std::cout << "Ban co muon thu lai khong? (y/n): ";
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice != 'y' && choice != 'Y') break;
        }
    } while(true);
}

void ConsoleUI::handleSearchProductByName() {
    char choice;
    do {
        clearScreen();
        std::cout << "Nhap ten san pham can tim: ";
        std::string nameQuery; std::getline(std::cin, nameQuery);
        std::transform(nameQuery.begin(), nameQuery.end(), nameQuery.begin(), ::tolower);
        std::vector<Product> results;
        for (const auto& p : service.getAllProducts()){
            std::string productName = p.getProductName();
            std::transform(productName.begin(), productName.end(), productName.begin(), ::tolower);
            if (productName.find(nameQuery) != std::string::npos){
                results.push_back(p);
            }
        }
        if(!results.empty()){
            std::cout << "\n=== KET QUA TIM KIEM ===" << std::endl;
            std::cout << std::left << std::setw(10) << "Ma SP" << std::setw(25) << "Ten SP" << std::setw(10) << "Don vi"
                      << std::setw(15) << "Don gia" << std::setw(10) << "So luong" << std::endl;
            std::cout << std::string(70, '-') << std::endl;
            for (const auto& p : results) {
                std::cout << std::left << std::setw(10) << p.getProductId() << std::setw(25) << p.getProductName()
                          << std::setw(10) << p.getUnit() << std::setw(15) << std::fixed << std::setprecision(0) << p.getPrice()
                          << std::setw(10) << p.getQuantity() << std::endl;
            }
        } else { std::cout << "Khong tim thay san pham nao co ten chua '" << nameQuery << "'." << std::endl; }
        std::cout << "\nBan co muon tim kiem ten khac? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice != 'y' && choice != 'Y') break;
    } while (true);
}

void ConsoleUI::handleImportProducts() {
    std::cout << "Nhap ten file can import (vi du: products1.txt): ";
    std::string filename;
    std::getline(std::cin, filename);

    if (filename.empty()) {
        std::cout << "Ten file khong duoc de trong." << std::endl;
        return;
    }

    // Dùng filesystem::path để nối đường dẫn một cách an toàn
    std::filesystem::path fullPath = std::filesystem::path(service.getDataPath()) / filename;

    ImportResult result = service.importProductsFromFile(fullPath.string());
    std::string logMessage = "Thuc hien import tu file '" + filename + "'. Ket qua - Them: " 
                           + std::to_string(result.importedCount) + ", Trung: " 
                           + std::to_string(result.duplicateCount) + ", Loi: " 
                           + std::to_string(result.missingFieldCount);
    Logger::getInstance().log(logMessage);

    if (result.importedCount == 0 && result.duplicateCount == 0 && result.missingFieldCount == 0) {
        std::cout << "Loi: Khong the mo file hoac file rong. Vui long kiem tra lai ten file: " << filename << std::endl;
    } else {
        std::cout << "--- KET QUA IMPORT ---" << std::endl;
        std::cout << "So san pham moi duoc them: " << result.importedCount << std::endl;
        std::cout << "So san pham bi trung ma (da bo qua): " << result.duplicateCount << std::endl;
        std::cout << "So dong bi loi hoac thieu du lieu (da bo qua): " << result.missingFieldCount << std::endl;
    }
}


// --- INVOICE UI ---
void ConsoleUI::handleInvoiceMenu() {
    Logger::getInstance().log("Truy cap Menu Quan Ly Hoa Don.");
    int choice;
    do {
        clearScreen();
        displayInvoiceMenu();
        choice = getChoice(0, 3);
        Logger::getInstance().log("Menu Hoa Don: Nguoi dung chon chuc nang " + std::to_string(choice));
        switch (choice) {
            case 1: handleCreateInvoice(); break;
            case 2: displayAllInvoices(); break;
            case 3: handleDisplayInvoiceById(); break;
        }
        if (choice != 0) pressEnterToContinue();
    } while (choice != 0);
}

void ConsoleUI::displayAllInvoices() {
    clearScreen(); const auto& invoices = service.getAllInvoices();
    std::cout << "\n=== DANH SACH HOA DON ===" << std::endl;
    if (invoices.empty()){ std::cout << "Chua co hoa don nao." << std::endl; return; }
    std::cout << std::left << std::setw(15) << "Ma hoa don" << std::setw(25) << "Ngay lap" << std::setw(20) << "Tong tien (VND)" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    for (const auto& inv : invoices) {
        std::cout << std::left << std::setw(15) << inv.getInvoiceId() << std::setw(25) << inv.getDate()
                  << std::setw(20) << std::fixed << std::setprecision(0) << inv.getTotalAmount() << std::endl;
    }
}

void ConsoleUI::handleCreateInvoice() {
    clearScreen(); std::cout << "=== TAO HOA DON MOI ===" << std::endl;
    std::string id;
    do {
        std::cout << "Nhap ma hoa don (so nguyen): "; std::getline(std::cin, id);
        if(!isInteger(id)) { std::cout << "Ma hoa don phai la so nguyen.\n"; continue; }
        if (service.isInvoiceIdTaken(id)) { std::cout << "Ma hoa don da ton tai.\n"; }
    } while (!isInteger(id) || service.isInvoiceIdTaken(id));

    std::string date_input;
    while(true) {
        std::cout << "Nhap ngay lap (dd/mm/yyyy, de trong de lay ngay hien tai): ";
        std::getline(std::cin, date_input);

        // Trường hợp 1: Người dùng không nhập gì, lấy ngày giờ hệ thống
        if (date_input.empty()) {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            char date_str[20];
            std::strftime(date_str, sizeof(date_str), "%d/%m/%Y", std::localtime(&in_time_t));
            date_input = date_str;
            break; // Ngày hợp lệ, thoát vòng lặp
        }

        // Trường hợp 2: Người dùng có nhập, kiểm tra tính hợp lệ
        int day, month, year;
        if (sscanf(date_input.c_str(), "%d/%d/%d", &day, &month, &year) == 3 && isValidDate(day, month, year)) {
            break; // Ngày hoàn toàn hợp lệ, thoát vòng lặp
        }
        
        // Nếu không hợp lệ, thông báo lỗi và lặp lại
        std::cout << "Ngay hoac dinh dang khong hop le. Vui long nhap lai (dd/mm/yyyy)." << std::endl;
    }

    // Nối thêm giờ hiện tại vào ngày đã hợp lệ
    time_t now_time = std::time(nullptr);
    tm* ltm = std::localtime(&now_time);
    char timeStr[15];
    sprintf(timeStr, " %02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    date_input.append(timeStr);
    // ---------------------------------------------

    Invoice newInvoice(id, date_input);
    char addMore;
    do {
        displayAllProducts();
        std::cout << "\nNhap ma san pham de them vao hoa don: "; std::string pid; std::getline(std::cin, pid);
        Product* p = service.findProductById(pid);
        if(!p) { std::cout << "Ma san pham khong ton tai." << std::endl; }
        else {
            int quantity;
            std::cout << "Nhap so luong: ";
            while (!(std::cin >> quantity) || quantity <= 0) {
                 std::cout << "So luong phai la so nguyen duong. Vui long nhap lai: ";
                 std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (p->getQuantity() < quantity) {
                std::cout << "So luong trong kho khong du (con " << p->getQuantity() << ")." << std::endl;
            } else {
                newInvoice.addItem(OrderItem(*p, quantity));
                p->sell(quantity);
                std::cout << "Da them san pham '" << p->getProductName() << "' vao hoa don." << std::endl;
            }
        }
        std::cout << "Ban co muon them san pham khac? (y/n): "; std::cin >> addMore;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while(addMore == 'y' || addMore == 'Y');
    
    if(!newInvoice.getItems().empty()){
        service.createInvoice(newInvoice);
        std::string logMessage = "Tao hoa don moi thanh cong. ID: " + newInvoice.getInvoiceId() + ", Tong tien: " + std::to_string(newInvoice.getTotalAmount());
        Logger::getInstance().log(logMessage);
        std::cout << "\nHoa don da duoc tao thanh cong!" << std::endl;
        displayInvoiceDetails(newInvoice);
    } else {
        Logger::getInstance().log("Huy tao hoa don do khong co san pham.");
        std::cout << "\nHuy tao hoa don vi khong co san pham nao." << std::endl;
    }
}

void ConsoleUI::handleDisplayInvoiceById(){
    clearScreen(); std::cout << "Nhap ma hoa don can tim: ";
    std::string id; std::getline(std::cin, id);
    Invoice* inv = service.findInvoiceById(id);
    if(inv){ displayInvoiceDetails(*inv); }
    else { std::cout << "Khong tim thay hoa don co ma: " << id << std::endl; }
}

void ConsoleUI::displayInvoiceDetails(const Invoice& invoice) {
    std::cout << "\n=================================== HOA DON ====================================" << std::endl;
    std::cout << "Ma hoa don: " << invoice.getInvoiceId() << std::endl;
    std::cout << "Ngay lap: " << invoice.getDate() << std::endl;
    std::cout << std::string(82, '=') << std::endl;
    std::cout << std::left << std::setw(10) << "Ma SP" << std::setw(25) << "Ten SP" << std::setw(10) << "So luong" << std::setw(15) << "Don gia (VND)" << std::setw(15) << "Thanh tien (VND)" << std::endl;
    std::cout << std::string(82, '-') << std::endl;
    for (const auto& item : invoice.getItems()) {
        const Product& prod = item.getProduct();
        std::cout << std::left << std::setw(10) << prod.getProductId() << std::setw(25) << prod.getProductName()
                  << std::setw(10) << item.getQuantity() << std::setw(15) << std::fixed << std::setprecision(0) << prod.getPrice()
                  << std::setw(15) << std::fixed << std::setprecision(0) << item.getTotalPrice() << std::endl;
    }
    std::cout << std::string(82, '-') << std::endl;
    std::cout << "TONG TIEN: " << std::fixed << std::setprecision(0) << invoice.getTotalAmount() << " VND" << std::endl;
    std::cout << std::string(82, '=') << "\n" << std::endl;
}

// --- STATISTICS UI ---
// hàm về menu của chức năng thống kê 
void ConsoleUI::handleStatisticsMenu(){
    Logger::getInstance().log("Truy cap Menu Bao Cao Thong Ke.");
    int choice;
    do {
        clearScreen();
        displayStatisticsMenu();
        choice = getChoice(0, 2);
        switch (choice) {
            case 1: handleTotalRevenueMenu(); break;
            case 2: handleProductRevenueByMonth(); break;
        }
    } while (choice != 0);
}

void ConsoleUI::handleTotalRevenueMenu(){
    int choice;
    do {
        clearScreen();
        displayTotalRevenueMenu();
        choice = getChoice(0, 3);
        switch (choice) {
            case 1: handleRevenueByDate(); break;
            case 2: handleRevenueByMonth(); break;
            case 3: handleRevenueByYear(); break;
        }
        if (choice != 0) pressEnterToContinue();
    } while (choice != 0);
}

void ConsoleUI::handleRevenueByDate() {
    std::string dateStr;
    int day, month, year;

    while (true) {
        std::cout << "Nhap ngay can xem doanh thu (dd/mm/yyyy): ";
        std::getline(std::cin, dateStr);

        // Dùng sscanf để vừa kiểm tra định dạng, vừa trích xuất giá trị
        if (sscanf(dateStr.c_str(), "%d/%d/%d", &day, &month, &year) == 3) {
            // Nếu định dạng đúng, gọi hàm helper để kiểm tra tính logic
            if (isValidDate(day, month, year)) {
                break; // Ngày hoàn toàn hợp lệ, thoát khỏi vòng lặp
            } else {
                std::cout << "Ngay khong ton tai (vi du: thang 4 khong co ngay 31). Vui long nhap lai." << std::endl;
            }
        } else {
            std::cout << "Dinh dang ngay khong hop le. Vui long nhap lai (dd/mm/yyyy)." << std::endl;
        }
    }
    std::string queryDate = dateStr.substr(0, 10);

    Logger::getInstance().log("Thuc hien bao cao doanh thu theo ngay: " + date);
    // Gọi hàm service mới để lấy dữ liệu thống kê
    DailyStatistics stats = service.getStatisticsForDate(date);

    clearScreen();
    std::cout << "\n=== THONG KE DOANH THU NGAY " << date << " ===" << std::endl;

    if (stats.invoiceCount == 0) {
        std::cout << "Khong co hoa don nao duoc lap trong ngay nay." << std::endl;
    } else {
        // In ra bảng với định dạng bạn yêu cầu
        std::cout << std::left << std::setw(20) << "Ngay/Thang/Nam"
                  << std::setw(20) << "Tong so hoa don"
                  << std::setw(20) << "Doanh thu" << std::endl;
        std::cout << std::string(60, '-') << std::endl;

        std::cout << std::left << std::setw(20) << date
                  << std::setw(20) << stats.invoiceCount
                  << std::fixed << std::setprecision(0) << stats.totalRevenue << " VND" << std::endl;
    }
}

void ConsoleUI::handleRevenueByMonth() {
    std::string input;
    int month, year;
    while(true) {
        std::cout << "Nhap thang va nam (mm/yyyy): ";
        std::getline(std::cin, input);
        // Dùng sscanf để kiểm tra định dạng và trích xuất giá trị
        if (sscanf(input.c_str(), "%d/%d", &month, &year) == 2 && month >= 1 && month <= 12) {
            break; // Nếu hợp lệ, thoát khỏi vòng lặp
        }
        std::cout << "Dinh dang khong hop le. Vui long nhap lai." << std::endl;
    }

    Logger::getInstance().log("Thuc hien bao cao doanh thu theo thang: " + input);
    
    // Gọi hàm service mới
    auto dailyStats = service.getDailyStatisticsForMonth(month, year);
    
    clearScreen();
    std::cout << "\n=== DOANH THU THEO NGAY TRONG THANG " << month << "/" << year << " ===" << std::endl;

    if (dailyStats.empty()) {
        std::cout << "Khong co doanh thu trong thang nay." << std::endl;
        return;
    }
    
    // Vẽ header của bảng
    std::cout << std::left << std::setw(15) << "Ngay"
              << std::setw(20) << "Tong so hoa don"
              << std::setw(20) << "Doanh thu" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    double totalMonthRevenue = 0;
    // Lặp qua map và in ra từng dòng
    for (const auto& pair : dailyStats) {
        int day = pair.first;
        const DailyStatistics& stats = pair.second;

        std::cout << std::left << std::setw(15) << day
                  << std::setw(20) << stats.invoiceCount
                  << std::fixed << std::setprecision(0) << stats.totalRevenue << " VND" << std::endl;
        
        totalMonthRevenue += stats.totalRevenue;
    }
    
    // In dòng tổng kết cuối cùng
    std::cout << std::string(55, '-') << std::endl;
    std::cout << std::left << std::setw(35) << "Tong doanh thu thang:"
              << std::fixed << std::setprecision(0) << totalMonthRevenue << " VND" << std::endl;
}

void ConsoleUI::handleRevenueByYear() {
    std::string input;
    int year;
    while(true) {
        std::cout << "Nhap nam (yyyy): ";
        std::getline(std::cin, input);
        // Kiểm tra độ dài là 4 và tất cả đều là số
        if (input.length() == 4 && isInteger(input)) {
            try {
                year = std::stoi(input);
                break; // Nếu hợp lệ, thoát khỏi vòng lặp
            } catch (...) {
                // Bỏ qua và lặp lại nếu stoi thất bại (trường hợp hiếm)
            }
        }
        std::cout << "Nam khong hop le. Vui long nhap dung 4 chu so." << std::endl;
    }

    Logger::getInstance().log("Thuc hien bao cao doanh thu theo nam: " + input);

    // Gọi hàm service mới
    auto monthlyStats = service.getMonthlyStatisticsForYear(year);

    clearScreen();
    std::cout << "\n=== DOANH THU THEO THANG TRONG NAM " << year << " ===" << std::endl;

    // Vẽ header của bảng
    std::cout << std::left << std::setw(15) << "Thang"
              << std::setw(20) << "Tong so hoa don"
              << std::setw(20) << "Doanh thu" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    double totalYearRevenue = 0;
    // Lặp qua tất cả 12 tháng để đảm bảo tháng nào cũng được in ra
    for (int m = 1; m <= 12; ++m) {
        std::cout << std::left << std::setw(15) << ("Thang " + std::to_string(m));

        // Kiểm tra xem tháng m có dữ liệu không
        if (monthlyStats.count(m)) {
            const auto& stats = monthlyStats.at(m);
            std::cout << std::setw(20) << stats.invoiceCount
                      << std::fixed << std::setprecision(0) << stats.totalRevenue << " VND" << std::endl;
            totalYearRevenue += stats.totalRevenue;
        } else {
            // Nếu không có dữ liệu, in ra số 0
            std::cout << std::setw(20) << 0
                      << std::fixed << std::setprecision(0) << 0.0 << " VND" << std::endl;
        }
    }

    // In dòng tổng kết cuối cùng
    std::cout << std::string(55, '-') << std::endl;
    std::cout << std::left << std::setw(35) << "Tong doanh thu nam:"
              << std::fixed << std::setprecision(0) << totalYearRevenue << " VND" << std::endl;
}

void ConsoleUI::handleProductRevenueByMonth() {
    std::string input;
    int month, year;
    while(true) {
        std::cout << "Nhap thang va nam (mm/yyyy): ";
        std::getline(std::cin, input);
        // Dùng sscanf để kiểm tra định dạng và trích xuất giá trị
        if (sscanf(input.c_str(), "%d/%d", &month, &year) == 2 && month >= 1 && month <= 12) {
            break; // Nếu hợp lệ, thoát khỏi vòng lặp
        }
        std::cout << "Dinh dang khong hop le. Vui long nhap lai." << std::endl;
    }

    Logger::getInstance().log("Thuc hien bao cao doanh thu san pham theo thang: " + input);

    auto results = service.getProductRevenueByMonth(month, year);
    if(results.empty()){ 
        std::cout << "Khong co san pham nao duoc ban trong thang nay." << std::endl; 
        pressEnterToContinue();
        return; }
    
    clearScreen();
    std::cout << "\n=== THONG KE SAN PHAM BAN RA THANG " << month << "/" << year << " ===" << std::endl;
    std::cout << std::left << std::setw(10) << "Ma SP" << std::setw(25) << "Ten SP"
              << std::setw(15) << "So luong ban" << std::setw(20) << "Don gia (VND)"
              << std::setw(20) << "Doanh thu (VND)" << std::setw(10) << "Trang thai" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    for(const auto& r : results){
        std::cout << std::left << std::setw(10) << r.id << std::setw(25) << r.name
                  << std::setw(15) << r.totalQuantity << std::setw(20) << std::fixed << std::setprecision(0) << r.priceAtTime
                  << std::setw(20) << std::fixed << std::setprecision(0) << r.totalRevenue
                  << std::setw(10) << (r.stillExists ? "Con ban" : "Da xoa") << std::endl;
    }
    pressEnterToContinue();
}