#include "employee.h"

#include "employeeList.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>

/*
author: anhnh
date: 28/11/2026
description: Khoi tao Employee voi cac gia tri mac dinh
*/
Employee::Employee()
    : employeeID(""), department(), fullName(""), position(""), salary(0.0) {}

/*
author: anhnh
date: 28/11/2026
description: Khoi tao Employee tu thong tin nhan vien va phong ban
*/
Employee::Employee(std::string employeeID, Department department,
                   std::string fullName, std::string position, double salary)
    : employeeID(std::move(employeeID)),
      department(std::move(department)),
      fullName(std::move(fullName)),
      position(std::move(position)),
      salary(salary) {}

/*
author: anhnh
date: 28/11/2026
description: Tra ve ma nhan vien
*/
std::string Employee::getEmployeeID() const { return employeeID; }

/*
author: anhnh
date: 28/11/2026
description: Tra ve phong ban cua nhan vien
*/
Department Employee::getDepartment() const { return department; }

/*
author: anhnh
date: 28/11/2026
description: Tra ve ho ten nhan vien
*/
std::string Employee::getFullName() const { return fullName; }

/*
author: anhnh
date: 28/11/2026
description: Tra ve chuc vu nhan vien
*/
std::string Employee::getPosition() const { return position; }

/*
author: anhnh
date: 28/11/2026
description: Tra ve muc luong cua nhan vien
*/
double Employee::getSalary() const { return salary; }

/*
author: anhnh
date: 28/11/2026
description: Cap nhat ma nhan vien
*/
void Employee::setEmployeeID(const std::string& value) {
    employeeID = value;
}

/*
author: anhnh
date: 28/11/2026
description: Cap nhat phong ban cua nhan vien
*/
void Employee::setDepartment(const Department& value) {
    department = value;
}

/*
author: anhnh
date: 28/11/2026
description: Cap nhat ho ten nhan vien
*/
void Employee::setFullName(const std::string& value) {
    fullName = value;
}

/*
author: anhnh
date: 28/11/2026
description: Cap nhat chuc vu nhan vien
*/
void Employee::setPosition(const std::string& value) {
    position = value;
}

/*
author: anhnh
date: 28/11/2026
description: Cap nhat muc luong nhan vien
*/
void Employee::setSalary(double value) {
    salary = value;
}

/*
author: anhnh
date: 28/11/2026
description: Dinh dang thong tin nhan vien thanh chuoi de hien thi
*/
std::string Employee::toString() const {
    std::ostringstream output;
    output << std::left
           << std::setw(8) << employeeID.substr(0, 8) << " | "
           << std::setw(30) << fullName.substr(0, 30) << " | "
           << std::setw(8) << department.getDepartmentID().substr(0, 8)
           << " | "
           << std::setw(30)
           << department.getDepartmentName().substr(0, 30) << " | "
           << std::setw(25) << position.substr(0, 25) << " | "
           << std::right << std::fixed << std::setprecision(2)
           << std::setw(15) << salary;
    return output.str();
}

/*
author: anhnh
date: 28/11/2026
description: So sanh hai nhan vien theo ma nhan vien
*/
bool Employee::operator<(const Employee& other) const {
    return employeeID < other.employeeID;
}

/*
author: anhnh
date: 28/11/2026
description: Tinh tong luong cua nhan vien thuoc mot phong ban
*/
double salaryByDepartment(const std::vector<Employee>& employees,
                          const std::string& departmentID) {
    double total = 0;
    for (const Employee& employee : employees)
        if (employee.getDepartment().getDepartmentID() == departmentID)
            total += employee.getSalary();
    return total;
}

/*
author: anhnh
date: 28/11/2026
description: Chuyen nhan vien tu phong ban cu sang phong ban duoc chon
*/
void EmployeeList::moveDepartment(const std::string& fromID,
                                  const Department& target) {
    for (Employee& employee : itemArr)
        if (employee.getDepartment().getDepartmentID() == fromID)
            employee.setDepartment(target);
    hasChangeNotUpdated = true;
}

/*
author: anhnh
date: 28/11/2026
description: Xoa tat ca nhan vien thuoc ma phong ban duoc chon
*/
void EmployeeList::removeByDepartment(const std::string& departmentID) {
    const size_t oldSize = itemArr.size();
    itemArr.erase(
        std::remove_if(itemArr.begin(), itemArr.end(),
                       [&departmentID](const Employee& employee) {
                           return employee.getDepartment().getDepartmentID()
                               == departmentID;
                       }),
        itemArr.end());
    if (itemArr.size() != oldSize) hasChangeNotUpdated = true;
}
