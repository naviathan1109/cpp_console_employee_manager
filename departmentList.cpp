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
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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
Ho va ten: Hoang Duc Tuyen
Date: 28/11/2026
Mo ta: Tách bản ghi theo ký tự phân cách và loại bỏ khoảng trắng ở hai đầu mỗi trường.
*/
std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::stringstream input(line);
    std::string field;
    while (std::getline(input, field, delimiter)) fields.push_back(trim(field));
    return fields;
}

/*
Ho va ten: Hoang Duc Tuyen
Date: 28/11/2026
Mo ta: Yêu cầu người dùng xác nhận hoặc hủy thao tác.
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

/*
   author: K26DTCN078_nvan
   date: 24/11/2026
   description: Trả về chỉ mục của phòng ban có ID được yêu cầu hoặc -1.
*/
int DepartmentList::searchID(const std::string& id) {
    for (size_t i = 0; i < itemArr.size(); ++i)
        if (itemArr[i].getDepartmentID() == id) return static_cast<int>(i);
    return -1;
}


/*
   author: K26DTCN078_nvan
   date: 24/11/2026
   description: Hiển thị các giá trị cũ và được đề xuất, sau đó áp dụng chỉnh sửa sau khi xác nhận.
*/
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

/*
Ho va ten: Hoang Duc Tuyen
Date: 28/11/2026
Mo ta: Hiển thị danh sách và cho phép người dùng chọn phòng ban theo mã hợp lệ.
*/
Department DepartmentList::getUserChoice() {
    if (itemArr.empty()) return Department();
    listDepartments();
    do {
        std::string id = readRequired("Enter department ID: ");
        int index = searchID(id);
        if (index >= 0) return itemArr[index];
        std::cout << "Department ID does not exist.\n";
    } while (confirm("Try another department ID?"));
    return Department();
}

/*
Ho va ten: Hoang Duc Tuyen
Date: 28/11/2026
Mo ta: Xác nhận và xóa phòng ban được chọn khỏi danh sách.
*/
bool DepartmentList::removeDepartment() {
    std::string id = readRequired("Enter the department ID to remove: ");
    int index = searchID(id);
    if (index < 0) {
        std::cout << "Department not found.\n";
        return false;
    }

    if (!confirm("Confirm final deletion of department " + id + "?")) return false;
    itemArr.erase(itemArr.begin() + index);
    hasChangeNotUpdated = true;
    std::cout << "Department removed.\n";
    return true;
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