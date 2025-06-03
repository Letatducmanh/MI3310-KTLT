#include "SalesManager.h"
#include <iostream>
#include "menu.h" 

int main() {
    std::cout << "=== CHUONG TRINH QUAN LY HOA DON BAN HANG ===" << std::endl;
    std::cout << "Phien ban: 1.0" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    try {
        SalesManager manager;
        manager.loadAllData(); 
        runMenu(manager); 
    }
    catch (const std::exception& e) {
        std::cerr << "Loi: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Da xay ra loi khong xac dinh!" << std::endl;
        return 1;
    }
    
    return 0;
}