 /*
Ho va ten: Doan Tuan Anh
Ma sinh vien: K26DTCN001
Date: 17/11/2026
- Tạo class departmentList.cpp
- Triển khai hàm loadFromFile và saveToFile trong departmentList.cpp
- Bổ sung code gọi đến hàm loadFromFile và saveToFile ở main() trong employeeManager.cpp
*/

#include "departmentList.h"

#include <iostream>
#include <sstream>
#include <string>

namespace {

std::string trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    size_t start = value.find_first_not_of(whitespace);
    // Neu chuoi toan khoang trang thi tra ve chuoi rong
    if (start == std::string::npos) return "";
    size_t end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
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

} 

// namespace
DepartmentList::DepartmentList() = default;

bool DepartmentList::loadFromFile(const std::string&) {
    return false;
}

bool DepartmentList::saveToFile(const std::string&) {
    return false;
}

int DepartmentList::searchID(const std::string&) {
    return -1;
}

Department DepartmentList::getUserChoice() {
    return Department();
}

/*
   author: anhnq
   date: 23/11/2026
   description: Them moi mot phong ban vao danh sach sau khi kiem tra trung ID
*/
void DepartmentList::addDepartment() {
    // Nhap ma phong ban va kiem tra tinh duy nhat trong danh sach
    std::string id = readRequired("Department ID: ");
    if (searchID(id) >= 0) {
        std::cout << "Department ID already exists.\n";
        return;
    }

    // Nhap cac thong tin con lai cua phong ban
    std::string name = readRequired("Department Name: ");
    std::string location = readRequired("Location: ");
    double budget = readPositiveDouble("Budget: ");

    // Them phong ban moi vao danh sach va danh dau co thay doi chua luu
    itemArr.emplace_back(id, name, location, budget);
    hasChangeNotUpdated = true;
    std::cout << "Department added.\n";
}

/*
   author: anhnq
   date: 23/11/2026
   description: Hien thi toan bo danh sach phong ban ra man hinh
*/
void DepartmentList::listDepartments() const {
    // Kiem tra danh sach rong truoc khi in
    if (itemArr.empty()) {
        std::cout << "There are no departments.\n";
        return;
    }

    // Duyet va in thong tin tung phong ban
    for (const Department& department : itemArr)
        std::cout << department.toString() << '\n';
}

/*
   author: anhdt
   date: 23/11/2026
   description: Loads pipe-delimited departments, validating their fields and unique IDs.
*/
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

/*
   author: anhdt
   date: 23/11/2026
   description: Saves all departments and marks the list clean only after a successful write.
*/
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