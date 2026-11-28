#include "employeeList.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>

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
    while (std::getline(input, field, delimiter))
        fields.push_back(trim(field));
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

// Converts a string to lowercase for case-insensitive department-name matching.
std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char character) {
                       return static_cast<char>(std::tolower(character));
                   });
    return value;
}
}

// Binds this employee list to its department list.
EmployeeList::EmployeeList(DepartmentList& departments)
    : EntityList<Employee>(), departmentList(departments) {}

// Loads employees and verifies record fields, IDs, and department references.
bool EmployeeList::loadFromFile(const std::string& filename) {
    std::ifstream input(filename);
    if (!input) throw std::runtime_error("Could not open employee file: " + filename);

    itemArr.clear();
    std::string line;
    while (std::getline(input, line)) {
        if (trim(line).empty()) continue;
        std::vector<std::string> fields = split(line, '|');
        if (fields.size() != 5)
            throw std::runtime_error("Invalid record format in employee file.");

        double salary = 0;
        std::stringstream parser(fields[4]);
        if (!(parser >> salary) || !parser.eof())
            throw std::runtime_error("Invalid employee salary.");

        int departmentIndex = departmentList.searchID(fields[3]);
        if (departmentIndex < 0)
            throw std::runtime_error("Employee references an unknown department ID: " + fields[3]);
        Department department = departmentList.getDepartment(departmentIndex);

        if (searchID(fields[0]) != -1)
            throw std::runtime_error("Duplicate employee ID: " + fields[0]);

        itemArr.emplace_back(fields[0], department, fields[1], fields[2], salary);
    }

    hasLoadFromFile = true;
    hasChangeNotUpdated = false;
    return true;
}

// Saves employees by department ID and marks the list clean after success.
bool EmployeeList::saveToFile(const std::string& filename) {
    std::ofstream output(filename);
    if (!output) return false;
    for (const Employee& employee : itemArr) {
        output << employee.getEmployeeID() << '|'
               << employee.getFullName() << '|'
               << employee.getPosition() << '|'
               << employee.getDepartment().getDepartmentID() << '|'
               << std::fixed << std::setprecision(2) << employee.getSalary() << '\n';
    }
    hasChangeNotUpdated = false;
    return true;
}

// Returns the index of the employee with the requested ID, or -1.
int EmployeeList::searchID(const std::string& id) {
    for (size_t i = 0; i < itemArr.size(); ++i)
        if (itemArr[i].getEmployeeID() == id) return static_cast<int>(i);
    return -1;
}

// Calculates the combined salary for employees assigned to a department.
double salaryByDepartment(const std::vector<Employee>& employees,
                          const std::string& departmentID) {
    double total = 0;
    for (const Employee& employee : employees)
        if (employee.getDepartment().getDepartmentID() == departmentID)
            total += employee.getSalary();
    return total;
}

// Creates an employee after selecting a department and checking its budget.
void EmployeeList::addEmployee() {
    Department department = departmentList.getUserChoice();
    if (department.getDepartmentID().empty()) {
        std::cout << "No valid department is available. Employee creation cancelled.\n";
        return;
    }

    std::string id = readRequired("Employee ID: ");
    if (searchID(id) >= 0) {
        std::cout << "Employee ID already exists.\n";
        return;
    }
    std::string name = readRequired("Full Name: ");
    std::string position = readRequired("Position: ");
    double salary = readPositiveDouble("Salary: ");

    double oldTotal = salaryByDepartment(itemArr, department.getDepartmentID());
    double newTotal = oldTotal + salary;
    if (newTotal > department.getBudget())
        if (!confirm("The department's total salary will exceed its budget (" +
                     std::to_string(newTotal) + " / " +
                     std::to_string(department.getBudget()) +
                     "). Continue?"))
            return;

    itemArr.emplace_back(id, department, name, position, salary);
    hasChangeNotUpdated = true;
    std::cout << "Employee added.\n";
}

// Removes the requested employee only after showing the record and confirmation.
bool EmployeeList::removeEmployee() {
    std::string id = readRequired("Enter the employee ID to remove: ");
    int index = searchID(id);
    if (index < 0) {
        std::cout << "Employee not found.\n";
        return false;
    }

    std::cout << "Employee to remove:\n" << itemArr[index].toString() << '\n';
    if (!confirm("Confirm employee removal?")) return false;
    itemArr.erase(itemArr.begin() + index);
    hasChangeNotUpdated = true;
    std::cout << "Employee removed.\n";
    return true;
}

// Updates an employee after displaying the before/after values and budget impact.
bool EmployeeList::updateEmployee() {
    std::string id = readRequired("Enter the employee ID to update: ");
    int index = searchID(id);
    if (index < 0) {
        std::cout << "Employee not found.\n";
        return false;
    }

    Employee before = itemArr[index];
    Department department = departmentList.getUserChoice();
    if (department.getDepartmentID().empty()) return false;

    std::string name = readRequired("Full Name moi: ");
    std::string position = readRequired("Position moi: ");
    double salary = readPositiveDouble("Salary moi: ");

    bool sameDepartment =
        before.getDepartment().getDepartmentID() == department.getDepartmentID();
    double oldTotal = salaryByDepartment(itemArr, department.getDepartmentID());
    double newTotal = sameDepartment
        ? oldTotal - before.getSalary() + salary
        : oldTotal + salary;

    Employee after(id, department, name, position, salary);
    std::cout << "\nBefore:\n" << before.toString()
              << "\nAfter:\n" << after.toString() << '\n';
    if (!confirm("Confirm employee update?")) return false;

    if (newTotal > department.getBudget())
        if (!confirm("The department's total salary will exceed its budget. Continue?"))
            return false;

    itemArr[index] = after;
    hasChangeNotUpdated = true;
    std::cout << "Employee updated.\n";
    return true;
}

// Prints employees in ascending employee-ID order.
void EmployeeList::listEmployees() const {
    if (itemArr.empty()) {
        std::cout << "There are no employees.\n";
        return;
    }
    std::vector<Employee> sorted = itemArr;
    std::sort(sorted.begin(), sorted.end());
    for (const Employee& employee : sorted)
        std::cout << employee.toString() << '\n';
}

// Finds and prints employees whose department name exactly matches the input.
void EmployeeList::printBasedDepartmentName() {
    std::string name = toLower(readRequired("Enter department name: "));
    bool found = false;
    for (const Employee& employee : itemArr) {
        Department department = employee.getDepartment();
        if (toLower(department.getDepartmentName()) == name) {
            std::cout << employee.toString() << '\n';
            found = true;
        }
    }
    if (!found) std::cout << "No employees found for that department.\n";
}

// Counts the employees assigned to a department ID.
size_t EmployeeList::countByDepartment(const std::string& departmentID) const {
    size_t count = 0;
    for (const Employee& employee : itemArr)
        if (employee.getDepartment().getDepartmentID() == departmentID) ++count;
    return count;
}

// Reassigns employees from a department to the selected target department.
void EmployeeList::moveDepartment(const std::string& fromID,
                                  const Department& target) {
    for (Employee& employee : itemArr)
        if (employee.getDepartment().getDepartmentID() == fromID)
            employee.setDepartment(target);
    hasChangeNotUpdated = true;
}

// Erases all employees assigned to the given department ID.
void EmployeeList::removeByDepartment(const std::string& departmentID) {
    size_t oldSize = itemArr.size();
    itemArr.erase(
        std::remove_if(itemArr.begin(), itemArr.end(),
                       [&](const Employee& employee) {
                           return employee.getDepartment().getDepartmentID() == departmentID;
                       }),
        itemArr.end());
    if (itemArr.size() != oldSize) hasChangeNotUpdated = true;
}

// Updates employee snapshots so they reflect a department's edited details.
void EmployeeList::synchronizeDepartment(const Department& updatedDepartment) {
    bool changed = false;
    for (Employee& employee : itemArr) {
        if (employee.getDepartment().getDepartmentID()
            == updatedDepartment.getDepartmentID()) {
            employee.setDepartment(updatedDepartment);
            changed = true;
        }
    }
    if (changed) hasChangeNotUpdated = true;
}