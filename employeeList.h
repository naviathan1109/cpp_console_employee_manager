#pragma once

#include "departmentList.h"
#include "employee.h"
#include "entityList.h"

#include <string>

/*
   author: anhnq
   date: 23/11/2026
   description: Khai bao lop EmployeeList (cac ham duoc phan cong cho anhnq:
                addEmployee, updateEmployee; giu cac khai bao bat buoc de khong loi bien dich)
*/
class EmployeeList : public EntityList<Employee> {
private:
    // Tham chieu toi danh sach phong ban de lien ket nhan vien voi phong ban
    DepartmentList& departmentList;

public:
    // Bat buoc co constructor de khoi tao tham chieu departmentList
    explicit EmployeeList(DepartmentList& departmentList);

    // Bat buoc khai bao override 3 ham thuan ao tu EntityList de EmployeeList khong bi abstract
    bool loadFromFile(const std::string& filename) override;
    bool saveToFile(const std::string& filename) override;
    int searchID(const std::string& id) override;
    bool removeEmployee();

    // --- Phan nhiem vu cua Nguyen Quang Anh (anhnq) ---
    // Nhap thong tin, kiem tra ngan sach phong ban va them moi nhan vien
    void addEmployee();

    // Cap nhat thong tin nhan vien theo ID, hien thi so sanh truoc/sau va xac nhan
    bool updateEmployee();
};
