# CMakeLists.txt cho chương trình quản lý hóa đơn bán hàng
cmake_minimum_required(VERSION 3.10)

project(SalesManagement_Final VERSION 1.1)

# Thiết lập chuẩn C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Thiết lập các cờ biên dịch
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
# --- DANH SÁCH TOÀN BỘ FILE MÃ NGUỒN (cả .h và .cpp) ---
set(PROJECT_SOURCES
    main.cpp
    
    # Models
    src/models/Product/Product.h
    src/models/Product/Product.cpp
    src/models/OrderItem/OrderItem.h
    src/models/OrderItem/OrderItem.cpp
    src/models/Invoice/Invoice.h
    src/models/Invoice/Invoice.cpp
    
    # Persistence
    src/persistence/DataManager.h
    src/persistence/DataManager.cpp
    
    # Services
    src/services/SalesService.h
    src/services/SalesService.cpp
    src/services/SalesService_Product.cpp
    src/services/SalesService_Invoice.cpp
    src/services/SalesService_Statistics.cpp
    
    # UI
    src/ui/Console/ConsoleUI.h
    src/ui/Console/ConsoleUI.cpp
    src/ui/Menu/Menu.h
    src/ui/Menu/Menu.cpp

    # log 
    src/log/Logger.h
    src/log/Logger.cpp 
)

# Tạo executable từ danh sách nguồn đã khai báo
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Lệnh này phải đi sau add_executable khi dùng với ${PROJECT_NAME}
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Đặt tên cho file thực thi cuối cùng
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "SalesManagement")