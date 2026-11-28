#include "employeeList.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

double salaryByDepartment(const std::vector<Employee>& employees,
                          const std::string& departmentID);

namespace {

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char character) {
                       return static_cast<char>(std::tolower(character));
                   });
    return value;
}

void printEmployeeHeader() {
    std::cout << std::left
              << std::setw(8) << "Emp ID" << " | "
              << std::setw(30) << "Full Name" << " | "
              << std::setw(8) << "Dept ID" << " | "
              << std::setw(30) << "Department Name" << " | "
              << std::setw(25) << "Position" << " | "
              << std::right << std::setw(15) << "Salary" << '\n';
}

std::string trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    size_t start = value.find_first_not_of(whitespace);
    // Neu chuoi toan khoang trang thi tra ve chuoi rong
    if (start == std::string::npos) return "";
    size_t end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
}

/*
   author: anhnh
   date: 28/11/2026
   description: Tach dong du lieu theo ky tu phan cach va loai bo khoang trang hai dau
*/
std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::stringstream input(line);
    std::string field;
    while (std::getline(input, field, delimiter))
        fields.push_back(trim(field));
    return fields;
}

/*
   author: anhnq
   date: 23/11/2026
   description: Nhap chuoi bat buoc tu ban phim, khong cho phep bo trong
*/
std::string readRequired(const std::string& label) {
    std::string value;
    do {
        std::cout << label;
        std::getline(std::cin, value);
        value = trim(value);
        // Thong bao loi neu nguoi dung de trong
        if (value.empty()) std::cout << "This field cannot be empty.\n";
    } while (value.empty());
    return value;
}

/*
   author: anhnq
   date: 23/11/2026
   description: Nhap so thuc khong am (>= 0) va kiem tra tinh hop le
*/
double readPositiveDouble(const std::string& label) {
    double value;
    std::string input;
    do {
        std::cout << label;
        std::getline(std::cin, input);
        std::stringstream parser(input);
        // Chi chap nhan neu ep kieu thanh cong, khong co ky tu thua va >= 0
        if (parser >> value && parser.eof() && value >= 0) return value;
        std::cout << "Enter a valid number greater than or equal to zero.\n";
    } while (true);
}

/*
   author: anhnq
   date: 23/11/2026
   description: Yeu cau nguoi dung xac nhan Co (y) hoac Khong (n)
*/
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
} // namespace

EmployeeList::EmployeeList(DepartmentList& departmentList)
    : departmentList(departmentList) {}

/*
   author: anhnh
   date: 28/11/2026
   description: Doc danh sach nhan vien tu tep, kiem tra du lieu va lien ket phong ban theo ma
*/
bool EmployeeList::loadFromFile(const std::string& filename) {
    std::ifstream input(filename);
    if (!input)
        throw std::runtime_error("Could not open employee file: " + filename);

    std::vector<Employee> loadedEmployees;
    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        if (trim(line).empty()) continue;

        const std::vector<std::string> fields = split(line, '|');
        if (fields.size() != 5)
            throw std::runtime_error(
                "Invalid record format in employee file at line " +
                std::to_string(lineNumber) + ".");
        if (fields[0].empty() || fields[1].empty() || fields[2].empty() ||
            fields[3].empty()) {
            throw std::runtime_error(
                "Employee fields cannot be empty at line " +
                std::to_string(lineNumber) + ".");
        }

        double salary = 0.0;
        std::stringstream parser(fields[4]);
        if (!(parser >> salary) || !parser.eof() || !std::isfinite(salary) ||
            salary < 0.0) {
            throw std::runtime_error(
                "Invalid employee salary at line " +
                std::to_string(lineNumber) + ".");
        }

        const int departmentIndex = departmentList.searchID(fields[3]);
        if (departmentIndex < 0)
            throw std::runtime_error("Unknown department ID at line " +
                                     std::to_string(lineNumber) + ": " +
                                     fields[3]);

        const bool duplicateID =
            std::find_if(loadedEmployees.begin(), loadedEmployees.end(),
                         [&fields](const Employee& employee) {
                             return employee.getEmployeeID() == fields[0];
                         }) != loadedEmployees.end();
        if (duplicateID)
            throw std::runtime_error("Duplicate employee ID: " + fields[0]);

        loadedEmployees.emplace_back(
            fields[0], departmentList.getDepartment(departmentIndex),
            fields[1], fields[2], salary);
    }

    if (input.bad())
        throw std::runtime_error("Error while reading employee file: " + filename);

    itemArr.swap(loadedEmployees);
    hasLoadFromFile = true;
    hasChangeNotUpdated = false;
    return true;
}

/*
   author: anhnh
   date: 28/11/2026
   description: Ghi danh sach nhan vien theo dinh dang tep va xoa co thay doi sau khi ghi thanh cong
*/
bool EmployeeList::saveToFile(const std::string& filename) {
    std::ofstream output(filename);
    if (!output) return false;

    for (const Employee& employee : itemArr) {
        output << employee.getEmployeeID() << '|'
               << employee.getFullName() << '|'
               << employee.getPosition() << '|'
               << employee.getDepartment().getDepartmentID() << '|'
               << std::fixed << std::setprecision(2) << employee.getSalary()
               << '\n';
    }

    if (!output) return false;
    hasChangeNotUpdated = false;
    return true;
}

/*
   author: anhnq
   date: 23/11/2026
   description: Them moi mot nhan vien sau khi chon phong ban, kiem tra trung ID
                va canh bao neu tong luong vuot ngan sach phong ban
*/
void EmployeeList::addEmployee() {
    // Cho nguoi dung chon phong ban hop le cho nhan vien moi
    Department department = departmentList.getUserChoice();
    if (department.getDepartmentID().empty()) {
        std::cout << "No valid department is available. Employee creation cancelled.\n";
        return;
    }

    // Nhap ma nhan vien va kiem tra trung lap
    std::string id = readRequired("Employee ID: ");
    if (searchID(id) >= 0) {
        std::cout << "Employee ID already exists.\n";
        return;
    }

    // Nhap thong tin ca nhan, chuc vu va muc luong
    std::string name = readRequired("Full Name: ");
    std::string position = readRequired("Position: ");
    double salary = readPositiveDouble("Salary: ");

    // Tinh tong luong moi cua phong ban va kiem tra co vuot ngan sach (budget) khong
    double oldTotal = salaryByDepartment(itemArr, department.getDepartmentID());
    double newTotal = oldTotal + salary;
    if (newTotal > department.getBudget()) {
        // Neu vuot ngan sach, hoi xac nhan nguoi dung truoc khi them
        if (!confirm("The department's total salary will exceed its budget (" +
                     std::to_string(newTotal) + " / " +
                     std::to_string(department.getBudget()) +
                     "). Continue?"))
            return;
    }

    // Them nhan vien vao danh sach va danh dau co thay doi chua luu
    itemArr.emplace_back(id, department, name, position, salary);
    hasChangeNotUpdated = true;
    std::cout << "Employee added.\n";
}

/*
   author: anhnq
   date: 23/11/2026
   description: Cap nhat thong tin nhan vien theo ID, hien thi so sanh truoc/sau,
                kiem tra ngan sach phong ban va luu sau khi xac nhan
*/
bool EmployeeList::updateEmployee() {
    // Nhap ID nhan vien can cap nhat va tim vi tri trong danh sach
    std::string id = readRequired("Enter the employee ID to update: ");
    int index = searchID(id);
    if (index < 0) {
        std::cout << "Employee not found.\n";
        return false;
    }

    // Luu lai thong tin cu de hien thi so sanh va tinh toan chenh lech luong
    Employee before = itemArr[index];

    // Chon phong ban moi cho nhan vien
    Department department = departmentList.getUserChoice();
    if (department.getDepartmentID().empty()) return false;

    // Nhap cac thong tin cap nhat moi
    std::string name = readRequired("New Full Name: ");
    std::string position = readRequired("New Position: ");
    double salary = readPositiveDouble("New Salary: ");

    // Tinh tong luong phong ban dich sau khi cap nhat
    bool sameDepartment =
        before.getDepartment().getDepartmentID() == department.getDepartmentID();
    double oldTotal = salaryByDepartment(itemArr, department.getDepartmentID());
    double newTotal = sameDepartment
        ? oldTotal - before.getSalary() + salary
        : oldTotal + salary;

    // Hien thi du lieu truoc va sau cap nhat de nguoi dung doi chieu
    Employee after(id, department, name, position, salary);
    std::cout << "\nBefore:\n" << before.toString()
              << "\nAfter:\n" << after.toString() << '\n';

    // Yeu cau xac nhan cap nhat
    if (!confirm("Confirm employee update?")) return false;

    // Kiem tra neu tong luong vuot ngan sach phong ban thi xac nhan lan nua
    if (newTotal > department.getBudget()) {
        if (!confirm("The department's total salary will exceed its budget. Continue?"))
            return false;
    }

    // Ap dung thay doi va danh dau chua luu vao tep
    itemArr[index] = after;
    hasChangeNotUpdated = true;
    std::cout << "Employee updated.\n";
    return true;
}

/**
 * @brief Removes an employee from the employee list.
 *
 * Prompts the user to enter an employee ID, searches for the corresponding
 * employee, displays the employee's information, and requests confirmation
 * before removing the employee from the list.
 *
 * @return true if the employee is successfully removed; otherwise, false.
 *
 * @author Vu Tran Tuan
 * @date 2026-11-23 20:41 (GMT+07:00)
 */
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

/**
 * @brief Searches for an employee by employee ID.
 *
 * Iterates through the employee list and compares each employee's ID
 * with the requested ID.
 *
 * @param id The employee ID to search for.
 * @return The index of the employee if found; otherwise, returns -1.
 *
 * @author Vu Tran Tuan
 * @date 2026-11-23 20:48 (GMT+07:00)
 */
// Returns the index of the employee with the requested ID, or -1.
int EmployeeList::searchID(const std::string& id) {
    for (size_t i = 0; i < itemArr.size(); ++i)
        if (itemArr[i].getEmployeeID() == id) return static_cast<int>(i);
    return -1;
}

/*
   author: anhnh
   date: 28/11/2026
   description: Dem so nhan vien dang duoc gan cho ma phong ban
*/
std::size_t EmployeeList::countByDepartment(
    const std::string& departmentID) const {
    std::size_t count = 0;
    for (const Employee& employee : itemArr)
        if (employee.getDepartment().getDepartmentID() == departmentID)
            ++count;
    return count;
}

/*
   author: anhnh
   date: 28/11/2026
   description: Cap nhat thong tin phong ban luu trong cac nhan vien thuoc phong ban do
*/
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

/*
   author: K26DTCN078_nvan
   date: 26/11/2026
   description: In nhân viên theo thứ tự ID nhân viên tăng dần.
*/
void EmployeeList::listEmployees() const {
    if (itemArr.empty()) {
        std::cout << "There are no employees.\n";
        return;
    }
    std::vector<Employee> sorted = itemArr;
    std::sort(sorted.begin(), sorted.end());
    printEmployeeHeader();
    for (const Employee& employee : sorted)
        std::cout << employee.toString() << '\n';
}

/*
   author: K26DTCN078_nvan
   date: 26/11/2026
   description: Tìm và in nhân viên có tên bộ phận khớp chính xác với dữ liệu đầu vào.
*/
void EmployeeList::printBasedDepartmentName() {
    std::string name = toLower(readRequired("Enter department name: "));
    bool found = false;
    for (const Employee& employee : itemArr) {
        Department department = employee.getDepartment();
        if (toLower(department.getDepartmentName()) == name) {
            if (!found) printEmployeeHeader();
            std::cout << employee.toString() << '\n';
            found = true;
        }
    }
    if (!found) std::cout << "No employees found for that department.\n";
}