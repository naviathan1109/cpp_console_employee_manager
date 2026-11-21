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
                    // Option 1: list all departments.
                    break;

                case 2:
                    // Option 2: add a department.
                    break;

                case 3:
                    // Option 3: search for a department by ID.
                    break;

                case 4:
                    // Option 4: update a department.
                    break;

                case 5:
                    // Option 5: remove a department.
                    break;

                case 6:
                    // Option 6: save departments to department.txt.
                    break;

                case 7:
                    // Option 7: list all employees.
                    break;

                case 8:
                    // Option 8: search employees by department name.
                    break;

                case 9:
                    // Option 9: add an employee.
                    break;

                case 10:
                    // Option 10: update an employee.
                    break;

                case 11:
                    // Option 11: remove an employee.
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
