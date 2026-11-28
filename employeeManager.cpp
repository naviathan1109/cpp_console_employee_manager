#include "employeeManager.h"

#include "departmentList.h"
#include "employeeList.h"
#include "menu.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {
// Removes leading and trailing whitespace from user input.
std::string trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    size_t start = value.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
}

// Reads a yes/no response and repeats until it is valid.
bool confirm(const std::string& message) {
    std::string answer;
    do {
        std::cout << message << " (y/n): ";
        std::getline(std::cin, answer);
        answer = trim(answer);
        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
        std::cout << "Enter y or n.\n";
    } while (true);
}

// Reads a non-empty line, trimming surrounding whitespace.
std::string readRequired(const std::string& label) {
    std::string value;
    do {
        std::cout << label;
        std::getline(std::cin, value);
        value = trim(value);
        if (value.empty()) std::cout << "This field cannot be empty.\n";
    } while (value.empty());
    return value;
}
}

// Loads persisted data, dispatches menu actions, and prompts before exiting.
void EmployeeManager::run() {
    DepartmentList departmentList;
    EmployeeList employeeList(departmentList);

    try {
        departmentList.loadFromFile("department.txt");
    } catch (const std::exception& error) {
        std::cout << "Error loading department.txt: " << error.what() << '\n';
    }

    try {
        employeeList.loadFromFile("employee.txt");
    } catch (const std::exception& error) {
        std::cout << "Error loading employee.txt: " << error.what() << '\n';
    }

    const std::vector<std::string> options = {
        "Exit",
        "List all Department",
        "Add a new Department",
        "Search Department by ID",
        "Update Department",
        "Remove Department",
        "Save Department to department.txt",
        "List all Employee",
        "Search employees by department name",
        "Add a new Employee",
        "Update Employee",
        "Remove Employee",
        "Save Employee to employee.txt"
    };

    Menu menu;
    while (true) {
        int choice = menu.int_getChoice(options);
        if (choice == 0) {
            if (departmentList.hasUnsavedChanges() || employeeList.hasUnsavedChanges()) {
                if (confirm("There are unsaved changes. Save before exiting?")) {
                    if (departmentList.hasUnsavedChanges())
                        departmentList.saveToFile("department.txt");
                    if (employeeList.hasUnsavedChanges())
                        employeeList.saveToFile("employee.txt");
                }
            }
            if (!confirm("Confirm exit?")) continue;
            break;
        }

        switch (choice) {
            case 1: departmentList.listDepartments(); break;
            case 2: departmentList.addDepartment(); break;
            case 3: {
                std::string id = readRequired("Enter department ID: ");
                int index = departmentList.searchID(id);
                if (index >= 0)
                    std::cout << departmentList.getDepartment(index).toString() << '\n';
                else
                    std::cout << "Department not found.\n";
                break;
            }
            case 4: departmentList.updateDepartment(employeeList); break;
            case 5: {
                std::string id = readRequired("Enter the department ID to remove: ");
                departmentList.removeDepartment(id, employeeList);
                break;
            }
            case 6:
                std::cout << (departmentList.saveToFile("department.txt")
                                  ? "Saved department.txt.\n"
                                  : "Could not save department.txt.\n");
                break;
            case 7: employeeList.listEmployees(); break;
            case 8: employeeList.printBasedDepartmentName(); break;
            case 9: employeeList.addEmployee(); break;
            case 10: employeeList.updateEmployee(); break;
            case 11: employeeList.removeEmployee(); break;
            case 12:
                std::cout << (employeeList.saveToFile("employee.txt")
                                  ? "Saved employee.txt.\n"
                                  : "Could not save employee.txt.\n");
                break;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

// Creates the manager and starts the console application.
int main() {
    EmployeeManager manager;
    manager.run();
    return 0;
}