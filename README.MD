#  MI3310-Building-Simple-Sales-Management-Program

This is our large project to build a sales management program in C++, with core functionalities such as create/delete products, create invoices, ...

## Prerequisites:

- C++11–compatible compiler (e.g. g++ 7+, clang 5+, MSVC 2017+)
- CMake 3.10+
- (Optional) Ninja or Make 

##  What to do:

 -  First Clone this project to your local device. Use `Main` branch, select `Code -> Download ZIP` or use with your Github Desktop.

 -  If you are just a beginner wants to try our project, go to `build` and run `SalesManagement.exe`.

 -  If you want to explore more: Should you change the code, you must rebuild the exe using
    - if you don't have folder build , you can type `mkdir build` in bash 
	- `cd build` if you are not in `build` folder

	- add new .cpp file to `CMakeLists.txt ` if you create new .cpp file

	- use CMakeTools `cmake --build .` 
	


  

##  What you can do:

- Add/Update products , delete products, find the products by the ID or name of products ,display all the information of the products,  read the external file in the folder data to add the products
- Create Invoices, display all the invoices , find the invoices from the ID Invoices 
- Daily/Monthly/Yearly sales statistics, Revenue statistics by product


  

##  Credits:

Special thanks to @TranHuyHoang (initial Products, OrderItem, Invoice structures ; Install modules: Product Manager, processing import and export of product data and invoices.) for the project.

  

##  Contributions:

Please open an issue or submit a pull request.

Feel free to suggest new features or report bugs!
