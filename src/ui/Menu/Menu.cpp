#include "ui/Menu/Menu.h"
#include <iostream>

void displayMainMenu() {
    std::cout << "\n=============== QUAN LY HOA DON BAN HANG ===============" << std::endl;
    std::cout << "1. Quan ly san pham" << std::endl;
    std::cout << "2. Quan ly hoa don" << std::endl;
    std::cout << "3. Bao cao thong ke" << std::endl;
    std::cout << "0. Thoat chuong trinh" << std::endl;
    std::cout << "=======================================================" << std::endl;
}

void displayProductMenu() {
    std::cout << "\n=============== QUAN LY SAN PHAM ===============" << std::endl;
    std::cout << "1. Them san pham" << std::endl;
    std::cout << "2. Sua san pham" << std::endl;
    std::cout << "3. Xoa san pham" << std::endl;
    std::cout << "4. Hien thi danh sach san pham" << std::endl;
    std::cout << "5. Tim kiem san pham" << std::endl;
    std::cout << "6. Doc du lieu tu file de them san pham" << std::endl;
    std::cout << "0. Quay lai" << std::endl;
    std::cout << "===============================================" << std::endl;
}

void displaySearchProductMenu() {
    std::cout << "\n=============== TIM KIEM SAN PHAM ===============" << std::endl;
    std::cout << "1. Tim kiem theo Ma san pham" << std::endl;
    std::cout << "2. Tim kiem theo Ten san pham" << std::endl;
    std::cout << "0. Quay lai" << std::endl;
    std::cout << "===============================================" << std::endl;
}

void displayInvoiceMenu() {
    std::cout << "\n=============== QUAN LY HOA DON ===============" << std::endl;
    std::cout << "1. Tao hoa don moi" << std::endl;
    std::cout << "2. Hien thi danh sach hoa don" << std::endl;
    std::cout << "3. Tim va hien thi hoa don theo ma" << std::endl;
    std::cout << "0. Quay lai" << std::endl;
    std::cout << "===============================================" << std::endl;
}

void displayStatisticsMenu() {
    std::cout << "\n=============== BAO CAO THONG KE ===============" << std::endl;
    std::cout << "1. Tong doanh thu (theo Ngay/Thang/Nam)" << std::endl;
    std::cout << "2. Thong ke doanh thu theo san pham (theo Thang)" << std::endl;
    std::cout << "0. Quay lai" << std::endl;
    std::cout << "===============================================" << std::endl;
}

void displayTotalRevenueMenu() {
    std::cout << "\n=============== TONG DOANH THU ===============" << std::endl;
    std::cout << "1. Tong doanh thu theo Ngay" << std::endl;
    std::cout << "2. Tong doanh thu theo Thang" << std::endl;
    std::cout << "3. Tong doanh thu theo Nam" << std::endl;
    std::cout << "0. Quay lai" << std::endl;
    std::cout << "==============================================" << std::endl;
}