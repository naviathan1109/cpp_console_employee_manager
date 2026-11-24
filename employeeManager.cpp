#include "departmentList.h"
#include "employeeList.h"
#include "menu.h"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

/*
   author: anhnh
   date: 19/11/2026
   description: Khoi tao danh sach chuc nang va chay vong lap xu ly lua chon
*/
int main() {
    const std::vector<std::string> options = {
        "Exit",
        "List all departments",
        "Add a department",
        "Search department by ID",
        "Update a department",
        "Remove a department",
        "Save departments to department.txt",
        "List all employees",
        "Search employees by department name",
        "Add an employee",
        "Update an employee",
        "Remove an employee",
        "Save employees to employee.txt"
    };

    // Khoi tao doi tuong quan ly danh sach phong ban va danh sach nhan vien (author: anhnq)
    DepartmentList departmentList;
    EmployeeList employeeList(departmentList);

    try {
        departmentList.loadFromFile("department.txt");
    } catch (const std::exception& error) {
        std::cout << "Error loading department.txt: " << error.what() << '\n';
    }

    try {
        while (true) {
            const int selectedIndex = getMenuChoice(options);
            // std::cout << "Selected option index: " << selectedIndex << '\n';

            switch (selectedIndex) {
                case 0:
                    // Option 0: confirm whether to exit the program.
                    if (confirmExit()) return 0;
                    break;

                case 1:
                    // Option 1: hien thi toan bo danh sach phong ban (author: anhnq)
                    departmentList.listDepartments();
                    break;

                case 2:
                    // Option 2: them moi mot phong ban (author: anhnq)
                    departmentList.addDepartment();
                    break;

                case 3: {
                    // Option 3: search department by ID (author: K26DTCN078_nvan)
                    std::string id = readRequired("Enter department ID: ");
                    int index = departmentList.searchID(id);
                    if (index >= 0)
                        std::cout << departmentList.getDepartment(index).toString() << '\n';
                    else
                        std::cout << "Department not found.\n";
                    break;
                }

                case 4: 
					// Option 4: update a department (author: K26DTCN078_nvan)
                    departmentList.updateDepartment(employeeList); 
                    break;

                case 5:
                    // Option 5: remove a department.
                    break;

                case 6:
                    // Option 6: save departments to department.txt.
                    departmentList.saveToFile("department.txt");
                    break;

                case 7:
                    // Option 7: liệt kê tất cả nhân viên. (author: K26DTCN078_nvan)
                    employeeList.listEmployees();
                    break;

                case 8:
                    // Option 8: tìm kiếm nhân viên theo tên bộ phận. (author: K26DTCN078_nvan)
                    employeeList.printBasedDepartmentName();
                    break;

                case 9:
                    // Option 9: them moi mot nhan vien (author: anhnq)
                    employeeList.addEmployee();
                    break;

                case 10:
                    // Option 10: cap nhat thong tin nhan vien theo ID (author: anhnq)
                    employeeList.updateEmployee();
                    break;

                case 11:
                    // Option 11: remove an employee.
                    employeeList.removeEmployee();
                    break;

                case 12:
                    // Option 12: save employees to employee.txt.
                    break;
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Application stopped: " << error.what() << '\n';
        return 1;
    }
}
