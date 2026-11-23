 /*
Ho va ten: Doan Tuan Anh
Ma sinh vien: K26DTCN001
Date: 17/11/2026
- Tạo class departmentList.cpp
- Triển khai hàm loadFromFile và saveToFile trong departmentList.cpp
- Bổ sung code gọi đến hàm loadFromFile và saveToFile ở main() trong employeeManager.cpp
*/

#include "departmentList.h"

#include "employeeList.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace {
// Removes whitespace around a field read from the console or a file.
std::string trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    size_t start = value.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
}

// Splits a delimited record and trims each resulting field.
std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::stringstream input(line);
    std::string field;
    while (std::getline(input, field, delimiter)) fields.push_back(trim(field));
    return fields;
}

// Prompts repeatedly until the user provides a non-empty value.
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

// Reads a non-negative number and rejects malformed input.
double readPositiveDouble(const std::string& label) {
    double value;
    std::string input;
    do {
        std::cout << label;
        std::getline(std::cin, input);
        std::stringstream parser(input);
        if (parser >> value && parser.eof() && value >= 0) return value;
        std::cout << "Enter a valid number greater than or equal to zero.\n";
    } while (true);
}

// Reads and validates a yes/no confirmation.
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
}

// Initializes the shared entity-list state.
DepartmentList::DepartmentList() : EntityList<Department>() {}

// Exposes the shared item storage under the legacy department-specific name.
std::vector<Department>& DepartmentList::departments() { return itemArr; }

// Provides read-only access to the shared item storage.
const std::vector<Department>& DepartmentList::departments() const { return itemArr; }

// Loads pipe-delimited departments, validating their fields and unique IDs.
bool DepartmentList::loadFromFile(const std::string& filename) {
    std::ifstream input(filename);
    if (!input) throw std::runtime_error("Could not open department file: " + filename);

    itemArr.clear();
    std::string line;
    while (std::getline(input, line)) {
        if (trim(line).empty()) continue;
        std::vector<std::string> fields = split(line, '|');
        if (fields.size() != 4)
            throw std::runtime_error("Invalid record format in department file.");
        double budget = 0;
        std::stringstream parser(fields[3]);
        if (!(parser >> budget) || !parser.eof())
            throw std::runtime_error("Invalid department budget.");
        if (searchID(fields[0]) != -1)
            throw std::runtime_error("Duplicate department ID: " + fields[0]);
        itemArr.emplace_back(fields[0], fields[1], fields[2], budget);
    }

    hasLoadFromFile = true;
    hasChangeNotUpdated = false;
    return true;
}

// Saves all departments and marks the list clean only after a successful write.
bool DepartmentList::saveToFile(const std::string& filename) {
    std::ofstream output(filename);
    if (!output) return false;
    for (const Department& department : itemArr) {
        output << department.getDepartmentID() << '|'
               << department.getDepartmentName() << '|'
               << department.getLocation() << '|'
               << std::fixed << std::setprecision(2) << department.getBudget() << '\n';
    }
    hasChangeNotUpdated = false;
    return true;
}

// Returns the index of the department with the requested ID, or -1.
int DepartmentList::searchID(const std::string& id) {
    for (size_t i = 0; i < itemArr.size(); ++i)
        if (itemArr[i].getDepartmentID() == id) return static_cast<int>(i);
    return -1;
}

// Returns a department by index, using an empty value for invalid indexes.
Department DepartmentList::getDepartment(int index) const {
    if (index < 0 || index >= static_cast<int>(itemArr.size())) return Department();
    return itemArr[index];
}

// Displays departments and lets the user select one by ID.
Department DepartmentList::getUserChoice() {
    if (itemArr.empty()) return Department();
    listDepartments();
    std::string id;
    do {
        id = readRequired("Enter department ID: ");
        int index = searchID(id);
        if (index >= 0) return itemArr[index];
        std::cout << "Department ID does not exist.\n";
    } while (confirm("Try another department ID?"));
    return Department();
}

// Returns a copy of all department records.
std::vector<Department> DepartmentList::getDepartments() const { return itemArr; }

// Collects a new department and rejects duplicate IDs.
void DepartmentList::addDepartment() {
    std::string id = readRequired("Department ID: ");
    if (searchID(id) >= 0) {
        std::cout << "Department ID already exists.\n";
        return;
    }
    std::string name = readRequired("Department Name: ");
    std::string location = readRequired("Location: ");
    double budget = readPositiveDouble("Budget: ");
    itemArr.emplace_back(id, name, location, budget);
    hasChangeNotUpdated = true;
    std::cout << "Department added.\n";
}

// Shows the old and proposed values, then applies the edit after confirmation.
void DepartmentList::updateDepartment(EmployeeList& employeeList) {
    std::string id = readRequired("Enter the department ID to update: ");
    int index = searchID(id);
    if (index < 0) {
        std::cout << "Department not found.\n";
        return;
    }

    Department before = itemArr[index];
    std::string name = readRequired("New department name: ");
    std::string location = readRequired("New location: ");
    double budget = readPositiveDouble("New budget: ");

    Department after(id, name, location, budget);
    std::cout << "\nBefore: " << before.toString()
              << "\nAfter:  " << after.toString() << "\n";
    if (!confirm("Confirm department update?")) return;

    itemArr[index] = after;
    employeeList.synchronizeDepartment(after);
    hasChangeNotUpdated = true;
    std::cout << "Department updated.\n";
}

// Resolves dependent employees before deleting the department.
bool DepartmentList::removeDepartment(const std::string& id, EmployeeList& employeeList) {
    int index = searchID(id);
    if (index < 0) {
        std::cout << "Department not found.\n";
        return false;
    }

    if (employeeList.countByDepartment(id) > 0) {
        std::cout << "This department has " << employeeList.countByDepartment(id)
                  << " employee(s).\n";
        std::cout << "1. Move all employees to another department\n";
        std::cout << "2. Delete all employees in this department\n";
        std::cout << "0. Cancel\n";

        int choice = 0;
        std::string input;
        do {
            std::cout << "Enter your choice: ";
            std::getline(std::cin, input);
            std::stringstream parser(input);
            if (parser >> choice && parser.eof() && (choice >= 0 && choice <= 2)) break;
            std::cout << "Invalid choice.\n";
        } while (true);

        if (choice == 0) return false;
        if (choice == 1) {
            Department target = getUserChoice();
            if (target.getDepartmentID().empty() || target.getDepartmentID() == id) {
                std::cout << "Cancelled because no valid target department was selected.\n";
                return false;
            }
            employeeList.moveDepartment(id, target);
        } else {
            employeeList.removeByDepartment(id);
        }
    }

    if (!confirm("Confirm final deletion of department \"" + id + "\"?")) return false;
    itemArr.erase(itemArr.begin() + index);
    hasChangeNotUpdated = true;
    std::cout << "Department removed.\n";
    return true;
}

// Prints every department or reports that the list is empty.
void DepartmentList::listDepartments() const {
    if (itemArr.empty()) {
        std::cout << "There are no departments.\n";
        return;
    }
    for (const Department& department : itemArr)
        std::cout << department.toString() << '\n';
}