#include "SalesManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits> 
#include <algorithm>

// Statistics
void SalesManager::displayRevenueByDate() const {
    if (invoiceCount == 0) {
        std::cout << "\nKhong co du lieu doanh thu!" << std::endl;
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm 1 lần trước vòng lặp

    while (true) {
        char inputDate[20];
        std::cout << "Nhap ngay muon xem doanh thu (dd/mm/yyyy): ";
        std::cin.getline(inputDate, 20);

        // Kiểm tra định dạng ngày tháng năm
        if (!isValidDate(inputDate)) {
            std::cout << "Dinh dang ngay khong hop le! Vui long nhap lai (dd/mm/yyyy)." << std::endl;
            continue;
        }

        int invoiceCountByDate = 0;
        double revenueByDate = 0.0;

        for (int i = 0; i < invoiceCount; i++) {
            // so sánh 10 ký tự đầu (dd/mm/yyyy) 
            if (strncmp(invoices[i].getDate(), inputDate, 10) == 0) {
                invoiceCountByDate++;
                revenueByDate += invoices[i].getTotalAmount();
            }
        }

        if (invoiceCountByDate > 0) {
            std::cout << "\n=== DOANH THU NGAY " << inputDate << " ===" << std::endl;
            std::cout << std::left << std::setw(15) << "Ngay"
                      << std::setw(20) << "So hoa don"
                      << std::setw(20) << "Doanh thu" << std::endl;
            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << std::left << std::setw(15) << inputDate
                      << std::setw(20) << invoiceCountByDate
                      << std::setw(20) << std::fixed << std::setprecision(2)
                      << revenueByDate << std::endl;
            break; // Có dữ liệu thì thoát
        } else {
            std::cout << "Khong co hoa don nao trong ngay nay!" << std::endl;
            std::cout << "Ban co muon tiep tuc su dung chuc nang nay khong? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            } else {
                break;
            }
        }
    }
}

void SalesManager::displayRevenueByMonth() const {
    if (invoiceCount == 0) {
        std::cout << "\nKhong co du lieu hoa don!" << std::endl;
        return;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm 1 lần trước vòng lặp

    while (true) {
        char inputMonthYear[20];
        std::cout << "Nhap thang/nam muon thong ke (mm/yyyy): ";
        std::cin.getline(inputMonthYear, 20);

        // Nếu nhập quá dài, xóa bộ đệm
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ban da nhap qua nhieu ky tu! Vui long nhap lai (mm/yyyy)." << std::endl;
            continue;
        }

        // Kiểm tra định dạng mm/yyyy
        if (strlen(inputMonthYear) != 7 || inputMonthYear[2] != '/' ||
            !isdigit(inputMonthYear[0]) || !isdigit(inputMonthYear[1]) ||
            !isdigit(inputMonthYear[3]) || !isdigit(inputMonthYear[4]) ||
            !isdigit(inputMonthYear[5]) || !isdigit(inputMonthYear[6])) {
            std::cout << "Dinh dang thang/nam khong hop le! Vui long nhap lai (mm/yyyy)." << std::endl;
            continue; // Bắt nhập lại
        }

        // Chuẩn hóa input về đúng định dạng mm/yyyy
        int month = atoi(inputMonthYear);
        int year = atoi(inputMonthYear + 3);
        char fixedMonthYear[8];
        sprintf(fixedMonthYear, "%02d/%04d", month, year);

        // Thống kê sản phẩm bán ra trong tháng/năm
        char productIds[1000][20];
        char productNames[1000][100];
        int productQuantities[1000] = {0};
        double productRevenues[1000] = {0.0};
        double productPrices[1000] = {0.0}; 
        int uniqueProductCount = 0;
        bool hasData = false;

        for (int i = 0; i < invoiceCount; i++) {
            const char* date = invoices[i].getDate(); // dd/mm/yyyy
            // Kiểm tra định dạng ngày hợp lệ
            if (strlen(date) >= 10 && date[2] == '/' && date[5] == '/') {
                // So sánh tháng/năm của hóa đơn với fixedMonthYear
                if (strncmp(date + 3, fixedMonthYear, 7) == 0) {
                    hasData = true;
                    OrderItem* items = invoices[i].getItems();
                    int itemCount = invoices[i].getItemCount();

                    for (int j = 0; j < itemCount; j++) {
                        const char* currentProductId = items[j].getProduct().getProductId();
                        bool found = false;
                        for (int k = 0; k < uniqueProductCount; k++) {
                            if (strcmp(productIds[k], currentProductId) == 0) {
                                productQuantities[k] += items[j].getQuantity();
                                productRevenues[k] += items[j].getTotalPrice();
                                found = true;
                                break;
                            }
                        }
                        if (!found && uniqueProductCount < 1000) {
                            strcpy(productIds[uniqueProductCount], currentProductId);
                            strcpy(productNames[uniqueProductCount], items[j].getProduct().getProductName());
                            productQuantities[uniqueProductCount] = items[j].getQuantity();
                            productPrices[uniqueProductCount] = items[j].getProduct().getPrice(); // <-- Thêm dòng này
                            productRevenues[uniqueProductCount] = items[j].getTotalPrice();
                            uniqueProductCount++;
                        }
                    }
                }
            }
        }

        // Sắp xếp: trạng thái "con" lên trước, "xoa" xuống sau, trong cùng trạng thái thì số lượng bán ra giảm dần
        for (int i = 0; i < uniqueProductCount - 1; i++) {
            for (int j = i + 1; j < uniqueProductCount; j++) {
                bool status_i = (findProductById(productIds[i]) != nullptr); // true = còn
                bool status_j = (findProductById(productIds[j]) != nullptr); // true = còn

                // Ưu tiên trạng thái "con" lên trước, nếu cùng trạng thái thì số lượng bán ra giảm dần
                if (status_i < status_j ||
                   (status_i == status_j && productQuantities[i] < productQuantities[j])) {
                    // Hoán đổi số lượng
                    std::swap(productQuantities[i], productQuantities[j]);
                    // Hoán đổi doanh thu
                    std::swap(productRevenues[i], productRevenues[j]);
                    // Hoán đổi đơn giá
                    std::swap(productPrices[i], productPrices[j]);
                    // Hoán đổi mã sản phẩm
                    char tempId[20], tempName[100];
                    strcpy(tempId, productIds[i]);
                    strcpy(productIds[i], productIds[j]);
                    strcpy(productIds[j], tempId);
                    // Hoán đổi tên sản phẩm
                    strcpy(tempName, productNames[i]);
                    strcpy(productNames[i], productNames[j]);
                    strcpy(productNames[j], tempName);
                }
            }
        }


        if (hasData) {
            std::cout << "\n=== DANH SACH SAN PHAM BAN RA TRONG THANG " << fixedMonthYear << " ===" << std::endl;
            std::cout << std::left << std::setw(10) << "Ma SP"
                      << std::setw(25) << "Ten SP"
                      << std::setw(15) << "So luong ban"
                      << std::setw(15) << "Don gia"
                      << std::setw(20) << "Doanh thu" 
                      << std::setw(10) << "Trang thai" << std::endl;
            std::cout << "------------------------------------------------------------------------------------------------" << std::endl;

            for (int i = 0; i < uniqueProductCount; i++) {
                // Kiểm tra trạng thái sản phẩm 
                const char* status = (findProductById(productIds[i]) != nullptr) ? "con" : "xoa" ; 
                std::cout << std::left << std::setw(10) << productIds[i]
                          << std::setw(25) << productNames[i]
                          << std::setw(15) << productQuantities[i]
                          << std::setw(15) << std::fixed << std::setprecision(2) << productPrices[i]
                          << std::setw(20) << std::fixed << std::setprecision(2) << productRevenues[i] 
                          << std::setw(10) << status << std::endl;
            }
            std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
            break; // Thoát khi đã in dữ liệu
        } else {
            std::cout << "Khong co san pham nao duoc ban trong thang/nam nay!" << std::endl;
            std::cout << "Ban co muon tiep tuc su dung chuc nang nay khong? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            } else {
                break;
            }
        }
    }
}

double SalesManager::calculateTotalRevenue(int month, int year) const {
    double total = 0.0;
    for (int i = 0; i < invoiceCount; i++) {
        // Giả sử invoices[i].getDate() trả về "dd/mm/yyyy"
        const char* date = invoices[i].getDate();
        int invMonth = (date[3] - '0') * 10 + (date[4] - '0');
        int invYear = atoi(date + 6);
        if ((month == 0 || invMonth == month) && invYear == year) {
            total += invoices[i].getTotalAmount();
        }
    }
    return total;
}

void SalesManager::calculateTotalRevenueByMonthMenu() const {
    if (invoiceCount == 0) {
        std::cout << "\nKhong co du lieu hoa don!" << std::endl;
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm

    while (true) {
        char inputMonthYear[20];
        std::cout << "Nhap thang/nam muon tinh tong doanh thu (mm/yyyy): ";
        std::cin.getline(inputMonthYear, 20);

        // Kiểm tra định dạng mm/yyyy
        if (strlen(inputMonthYear) != 7 || inputMonthYear[2] != '/' ||
            !isdigit(inputMonthYear[0]) || !isdigit(inputMonthYear[1]) ||
            !isdigit(inputMonthYear[3]) || !isdigit(inputMonthYear[4]) ||
            !isdigit(inputMonthYear[5]) || !isdigit(inputMonthYear[6])) {
            std::cout << "Dinh dang thang/nam khong hop le! Vui long nhap lai (mm/yyyy)." << std::endl;
            continue;
        }

        int month = atoi(inputMonthYear);
        int year = atoi(inputMonthYear + 3);

        if (month < 1 || month > 12 || year < 1000 || year > 9999) {
            std::cout << "Thang hoac nam khong hop le! Vui long nhap lai (mm/yyyy)." << std::endl;
            continue;
        }

        // Đếm số hóa đơn hợp lệ
        int invoiceCountByMonth = 0;
        double total = 0.0;
        for (int i = 0; i < invoiceCount; i++) {
            const char* date = invoices[i].getDate();
            int invMonth = (date[3] - '0') * 10 + (date[4] - '0');
            int invYear = atoi(date + 6);
            if (invMonth == month && invYear == year) {
                total += invoices[i].getTotalAmount();
                invoiceCountByMonth++;
            }
        }

        if (total > 0) {
            std::cout << "\nTong doanh thu thang " << month << "/" << year << ": "
                      << std::fixed << std::setprecision(2) << total << " VND" << std::endl;
            std::cout << "So hoa don trong thang: " << invoiceCountByMonth << std::endl;

            // Thống kê doanh thu theo ngày
            char dayList[31][11] = {0}; // dd/mm/yyyy, tối đa 31 ngày
            int invoiceCountByDay[31] = {0};
            double revenueByDay[31] = {0.0};
            int uniqueDayCount = 0;

            for (int i = 0; i < invoiceCount; i++) {
                const char* date = invoices[i].getDate();
                int invMonth = (date[3] - '0') * 10 + (date[4] - '0');
                int invYear = atoi(date + 6);
                if (invMonth == month && invYear == year) {
                    // Kiểm tra ngày đã có trong dayList chưa
                    bool found = false;
                    for (int d = 0; d < uniqueDayCount; d++) {
                        if (strncmp(dayList[d], date, 10) == 0) {
                            invoiceCountByDay[d]++;
                            revenueByDay[d] += invoices[i].getTotalAmount();
                            found = true;
                            break;
                        }
                    }
                    if (!found && uniqueDayCount < 31) {
                        strncpy(dayList[uniqueDayCount], date, 10);
                        dayList[uniqueDayCount][10] = '\0';
                        invoiceCountByDay[uniqueDayCount] = 1;
                        revenueByDay[uniqueDayCount] = invoices[i].getTotalAmount();
                        uniqueDayCount++;
                    }
                }
            }

            // In bảng doanh thu theo ngày
            std::cout << "\n=== DOANH THU THEO NGAY TRONG THANG " << month << "/" << year << " ===" << std::endl;
            std::cout << std::left << std::setw(15) << "Ngay"
                      << std::setw(20) << "So hoa don"
                      << std::setw(20) << "Doanh thu" << std::endl;
            std::cout << "-------------------------------------------------------" << std::endl;
            for (int d = 0; d < uniqueDayCount; d++) {
                std::cout << std::left << std::setw(15) << dayList[d]
                          << std::setw(20) << invoiceCountByDay[d]
                          << std::setw(20) << std::fixed << std::setprecision(2) << revenueByDay[d] << std::endl;
            }
            break;
        } 
        else {
            std::cout << "Khong ton tai du lieu doanh thu cho thang/nam nay!" << std::endl;
            std::cout << "Ban co muon tiep tuc su dung chuc nang nay khong? (y/n): ";
            char choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice == 'y' || choice == 'Y') {
                continue;
            } else {
                break;
            }
        }
    }
}


void SalesManager::calculateTotalRevenueByYearMenu() const {
    if (invoiceCount == 0) {
        std::cout << "\nKhong co du lieu hoa don!" << std::endl;
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm

    while (true) {
        char inputYear[10];
        std::cout << "Nhap nam muon tinh tong doanh thu (yyyy): ";
        std::cin.getline(inputYear, 10);

        // Kiểm tra định dạng yyyy
        if (strlen(inputYear) != 4 ||
            !isdigit(inputYear[0]) || !isdigit(inputYear[1]) ||
            !isdigit(inputYear[2]) || !isdigit(inputYear[3])) {
            std::cout << "Dinh dang nam khong hop le! Vui long nhap lai (yyyy)." << std::endl;
            continue;
        }

        int year = atoi(inputYear);
        if (year < 1000 || year > 9999) {
            std::cout << "Nam khong hop le! Vui long nhap lai (yyyy)." << std::endl;
            continue;
        }

        // Đếm số hóa đơn hợp lệ và doanh thu từng tháng
        int invoiceCountByYear = 0;
        double total = 0.0;
        int invoiceCountByMonth[12] = {0};
        double revenueByMonth[12] = {0.0};

        for (int i = 0; i < invoiceCount; i++) {
            const char* date = invoices[i].getDate();
            if (strlen(date) >= 10) {
                int invMonth = (date[3] - '0') * 10 + (date[4] - '0');
                int invYear = atoi(date + 6);
                if (invYear == year && invMonth >= 1 && invMonth <= 12) {
                    total += invoices[i].getTotalAmount();
                    invoiceCountByYear++;
                    invoiceCountByMonth[invMonth - 1]++;
                    revenueByMonth[invMonth - 1] += invoices[i].getTotalAmount();
                }
            }
        }

        if (total > 0) {
            std::cout << "\nTong doanh thu nam " << year << ": "
                      << std::fixed << std::setprecision(2) << total << " VND" << std::endl;
            std::cout << "So hoa don trong nam: " << invoiceCountByYear << std::endl;

            // In bảng doanh thu theo tháng
            std::cout << "\n=== DOANH THU THEO THANG TRONG NAM " << year << " ===" << std::endl;
            std::cout << std::left << std::setw(10) << "Thang"
                      << std::setw(20) << "So hoa don"
                      << std::setw(20) << "Doanh thu" << std::endl;
            std::cout << "-------------------------------------------------------" << std::endl;
            for (int m = 0; m < 12; m++) {
                if (invoiceCountByMonth[m] > 0) {
                    std::cout << std::left << std::setw(10) << (m + 1)
                              << std::setw(20) << invoiceCountByMonth[m]
                              << std::setw(20) << std::fixed << std::setprecision(2) << revenueByMonth[m] << std::endl;
                }
            }
            break;
        } else {
            std::cout << "Khong ton tai du lieu doanh thu cho nam nay!" << std::endl;
            std::cout << "Ban co muon tiep tuc su dung chuc nang nay khong? (y/n): ";
            char choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice == 'y' || choice == 'Y') {
                continue;
            } else {
                break;
            }
        }
    }
}
