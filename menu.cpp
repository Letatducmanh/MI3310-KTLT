#include "menu.h"
#include "SalesManager.h"
#include <iostream>
#include <iomanip>
#include <limits> 

void displayMainMenu() {
    std::cout << "\n=============== QUAN LY HOA DON BAN HANG ===============" << std::endl;
    std::cout << "1. Quan ly san pham" << std::endl;
    std::cout << "2. Quan ly hoa don" << std::endl;
    std::cout << "3. Bao cao thong ke" << std::endl;
    std::cout << "0. Thoat chuong trinh" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "Lua chon cua ban: ";
}

void displayProductMenu() {
    std::cout << "\n=============== QUAN lY SAN PHAM ===============" << std::endl;
    std::cout << "1. Them san pham" << std::endl;
    std::cout << "2. Sua san pham" << std::endl;
    std::cout << "3. Xoa san pham" << std::endl;
    std::cout << "4. Hien thi danh sach san pham" << std::endl;
    std::cout << "5. Tim kiem san pham " << std::endl;
    std::cout << "6. Doc du lieu tu ben ngoai de them san pham" << std::endl;
    std::cout << "0. Quay lai" << std::endl;
    std::cout << "Lua chon cua ban: ";
}

void displayInvoiceMenu() {
    std::cout << "\n=============== QUAN LY HOA DON ===============" << std::endl;
    std::cout << "1. Tao hoa don moi" << std::endl;
    std::cout << "2. Hien thi danh sach hoa don" << std::endl;
    std::cout << "3. Tim hoa don theo ma" << std::endl;
    std::cout << "0. Quay lai" << std::endl;
    std::cout << "Lua chon cua ban: ";
}

void displayStatisticMenu() {
    std::cout << "\n=============== BAO CAO THONG KE ===============" << std::endl;
    std::cout << "1. Tong doanh thu " << std::endl;
    std::cout << "2. Thong ke doanh thu theo san pham" << std::endl;
    std::cout << "0. Quay lai" << std::endl;
    std::cout << "Lua chon cua ban: ";
}

void displayCalculateMenu () {
    std::cout << "\n=============== TONG DOANH THU ===============" << std::endl;
    std::cout << "1. Tong doanh thu theo ngay\n" ;
    std::cout << "2. Tong doanh thu theo thang\n" ;
    std::cout << "3. Tong doanh thu theo nam\n" ;
    std::cout << "0. Quay lai\n" ;
    std::cout << "Lua chon cua ban: ";
}

void displaySearchMenu () {
    std::cout << "\n=============== TIM KIEM SAN PHAM ===============" << std::endl;
    std::cout << "1. Tim kiem theo ten san pham\n" ; 
    std::cout << "2. Tim kiem theo Id san pham\n"; 
    std::cout << "0. Quay lai" << std::endl;
    std::cout << "Lua chon cua ban: ";
}

void runMenu(SalesManager& manager) {
    manager.loadAllData();
    manager.loadInvoicesFromFile();

    int mainChoice;
    do {
        // Vòng lặp kiểm tra hợp lệ cho main menu
        do {
            system("cls");
            displayMainMenu();
            std::cin >> mainChoice;
            if (std::cin.fail() || mainChoice < 0 || mainChoice > 3) {
                std::cout << "Lua chon khong hop le! Vui long chon lai." << std::endl;
                manager.clearInputBuffer();
            } else {
                break;
            }
        } while (true);

        switch (mainChoice) {
            case 1: { // Product management
                int subChoice;
                do {
                    // Vòng lặp kiểm tra hợp lệ cho product menu
                    do {
                        system("cls");
                        displayProductMenu();
                        std::cin >> subChoice;
                        if (std::cin.fail() || subChoice < 0 || subChoice > 6) {
                            std::cout << "Lua chon khong hop le! Vui long chon lai." << std::endl;
                            manager.clearInputBuffer();
                        } else {
                            break;
                        }
                    } while (true);

                    switch (subChoice) {
                        case 1:
                            system("cls");
                            manager.addProduct();
                            break;
                        case 2:
                            system("cls");
                            manager.editProduct();
                            std::cout << "\nNhan Enter de tiep tuc...";
                            std::cin.get();
                            break;
                        case 3:
                            system("cls");
                            manager.deleteProduct();
                            break;
                        case 4:
                            system("cls");
                            manager.displayAllProducts();
                            break;
                        case 5: {
                            int searchChoice;
                            do {
                                system("cls");
                                displaySearchMenu();
                                std::cin >> searchChoice;
                                manager.clearInputBuffer();
                                switch (searchChoice) {
                                    case 1:
                                        system("cls");
                                        manager.searchProductByName();
                                        std::cout << "\nNhan Enter de tiep tuc...";
                                        std::cin.get();
                                        break;
                                    case 2:
                                        system("cls");
                                        manager.searchProductById();
                                        std::cout << "\nNhan Enter de tiep tuc...";
                                        std::cin.get();
                                        break;
                                    case 0:
                                        break;
                                    default:
                                        std::cout << "Lua chon khong hop le! Vui long chon lai.\n";
                                        break;
                                }
                            } while (searchChoice != 0);
                            break;
                        }
                        case 6: 
                            system("cls");
                            char filename[100];
                            std::cout << "Nhap ten file can import: ";
                            std::cin >> filename;
                            manager.importProductsFromFile(filename);
                            break;
                             
                        case 0:
                            break;
                    }
                    if (subChoice != 0 && subChoice != 2 && subChoice != 5) {
                        std::cout << "\nNhan Enter de tiep tuc...";
                        manager.clearInputBuffer();
                        std::cin.get();
                    }
                } while (subChoice != 0);
                break;
            }
            case 2: { // Invoice management
                int subChoice;
                do {
                    // Vòng lặp kiểm tra hợp lệ cho invoice menu
                    do {
                        system("cls");
                        displayInvoiceMenu();
                        std::cin >> subChoice;
                        if (std::cin.fail() || subChoice < 0 || subChoice > 3) {
                            std::cout << "Lua chon khong hop le! Vui long chon lai." << std::endl;
                            manager.clearInputBuffer();
                        } else {
                            break;
                        }
                    } while (true);

                    switch (subChoice) {
                        case 1:
                            system("cls");
                            manager.createInvoice();
                            break;
                        case 2:
                            system("cls");
                            manager.displayAllInvoices();
                            break;
                        case 3: {
                            system("cls");
                            char id[20];
                            std::cout << "Nhap ma hoa don: ";
                            std::cin >> id;
                            manager.displayInvoiceById(id);
                            break;
                        }
                        case 0:
                            break;
                    }
                    if (subChoice != 0) {
                        std::cout << "\nNhan Enter de tiep tuc...";
                        manager.clearInputBuffer();
                        std::cin.get();
                    }
                } while (subChoice != 0);
                break;
            }
            case 3: { // Statistics
                int subChoice;
                do {
                    // Vòng lặp kiểm tra hợp lệ cho statistic menu
                    do {
                        system("cls");
                        displayStatisticMenu();
                        std::cin >> subChoice;
                        if (std::cin.fail() || subChoice < 0 || subChoice > 3) {
                            std::cout << "Lua chon khong hop le! Vui long chon lai." << std::endl;
                            manager.clearInputBuffer();
                        } else {
                            break;
                        }
                    } while (true);

                    switch (subChoice) {
                        case 2:
                            system("cls");
                            manager.displayRevenueByMonth();
                            std::cout << "\nNhan Enter de tiep tuc...";
                            std::cin.get();
                            break;
                        case 1: {
                            int choice;
                            do {
                                system("cls");
                                displayCalculateMenu();
                                std::cin >> choice;

                                switch (choice) {
                                    case 1:
                                        system("cls");
                                        manager.displayRevenueByDate();
                                        std::cout << "\nNhan Enter de tiep tuc...";
                                        std::cin.get();
                                        break;
                                    case 2:
                                        system("cls");
                                        manager.calculateTotalRevenueByMonthMenu();
                                        break;
                                    case 3:
                                        system("cls");
                                        manager.calculateTotalRevenueByYearMenu();
                                        break;
                                    case 0:
                                        break;
                                    default:
                                        std::cout << "Lua chon khong hop le!" << std::endl;
                                        break;
                                }
                                if (choice != 0 && choice != 1) {
                                    std::cout << "\nNhan Enter de tiep tuc...";
                                    std::cin.get();
                                }
                            } 
                            while (choice != 0);
                            break;
                        }
                        case 0:
                            break;
                    }
                    if (subChoice != 0 && subChoice != 2) {
                        std::cout << "\nNhan Enter de tiep tuc...";
                        std::cin.get();
                    }
                } while (subChoice != 0);
                break;
            }
            case 0:
                manager.saveAllData();
                std::cout << "Cam on ban da su dung chuong trinh!" << std::endl;
                break;
        }
        if (mainChoice != 0) {
            std::cout << "\nNhan Enter de tiep tuc...";
            manager.clearInputBuffer();
            std::cin.get();
        }
    } while (mainChoice != 0);
}
