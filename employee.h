#pragma once

#include "department.h"

#include <string>

// Stores an employee's identity, department, role, and salary.
class Employee {
private:
    std::string employeeID;
    Department department;
    std::string fullName;
    std::string position;
    double salary;

public:
    Employee();
    Employee(std::string employeeID, Department department,
             std::string fullName, std::string position, double salary);

    std::string getEmployeeID() const;
    Department getDepartment() const;
    std::string getFullName() const;
    std::string getPosition() const;
    double getSalary() const;

    void setEmployeeID(const std::string& value);
    void setDepartment(const Department& value);
    void setFullName(const std::string& value);
    void setPosition(const std::string& value);
    void setSalary(double value);

    std::string toString() const;
    bool operator<(const Employee& other) const;
};
