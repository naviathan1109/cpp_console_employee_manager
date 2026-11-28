#pragma once

#include "department.h"
#include "entityList.h"

#include <string>
#include <vector>

class EmployeeList;

/*
   author: anhnq
   date: 23/11/2026
   description: Khai bao lop DepartmentList (cac ham duoc phan cong cho anhnq:
                addDepartment, listDepartments; giu cac khai bao bat buoc de khong loi bien dich)
*/
class DepartmentList : public EntityList<Department> {
public:
    DepartmentList();

    // Bat buoc khai bao override 3 ham thuan ao tu EntityList de DepartmentList khong bi abstract
    bool loadFromFile(const std::string& filename) override;
    bool saveToFile(const std::string& filename) override;
    int searchID(const std::string& id) override;

    // Can khai bao vi EmployeeList::addEmployee() va updateEmployee() goi den ham nay
    Department getUserChoice();
    Department getDepartment(int index) const;
    void updateDepartment(EmployeeList& employeeList);
    bool removeDepartment(EmployeeList& employeeList);
    bool removeDepartment(const std::string& id, EmployeeList& employeeList);

    // --- Phan nhiem vu cua Nguyen Quang Anh (anhnq) ---
    // Nhap thong tin va them moi mot phong ban (kiem tra trung ma phong ban)
    void addDepartment();

    // Hien thi toan bo danh sach phong ban hien co
    void listDepartments() const;
};
