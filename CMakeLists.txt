# CMakeLists.txt cho chương trình quản lý hóa đơn bán hàng
cmake_minimum_required(VERSION 3.10)

# Thiết lập tên dự án
project(SalesManagement VERSION 1.0)

# Thiết lập chuẩn C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Thiết lập các cờ biên dịch
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Thiết lập các cờ debug và release
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

# Danh sách các file header
set(HEADERS
    Product.h
    OrderItem.h
    Invoice.h
    SalesManager.h
    menu.h
)

# Danh sách các file source
set(SOURCES
    Product.cpp
    OrderItem.cpp
    Invoice.cpp
    SalesManager.cpp
    main.cpp
    menu.cpp
    ProductManager.cpp
    InvoiceManager.cpp
    StatisticManager.cpp
    FileOperation.cpp  
)

# Tạo thư mục build nếu chưa có
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Thiết lập thư mục output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Tạo executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Thiết lập thuộc tính cho executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "SalesManagement"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

