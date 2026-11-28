#pragma once

#include "departmentList.h"
#include "employee.h"
#include "entityList.h"

#include <string>

// Manages employee records and their association with the department list.
class EmployeeList : public EntityList<Employee> {
private:
    DepartmentList& departmentList;

public:
    // Creates an employee list linked to the supplied department list.
    explicit EmployeeList(DepartmentList& departmentList);

    // Loads pipe-delimited employees and resolves each referenced department.
    bool loadFromFile(const std::string& filename) override;
    // Writes all employees and clears the unsaved-changes flag on success.
    bool saveToFile(const std::string& filename) override;
    // Finds an employee by ID and returns its index or -1.
    int searchID(const std::string& id) override;

    // Prompts for employee data and adds it after validation.
    void addEmployee();
    // Removes an employee after explicit confirmation.
    bool removeEmployee();
    // Shows proposed employee changes and applies them after confirmation.
    bool updateEmployee();
    // Prints employees ordered by ID.
    void listEmployees() const;
    // Prompts for a department name and lists employees assigned to it.
    void printBasedDepartmentName();

    // Counts employees assigned to the specified department.
    size_t countByDepartment(const std::string& departmentID) const;
    // Reassigns all employees from one department to another.
    void moveDepartment(const std::string& fromID, const Department& target);
    // Removes every employee assigned to the specified department.
    void removeByDepartment(const std::string& departmentID);
    // Refreshes stored department values after a department is edited.
    void synchronizeDepartment(const Department& updatedDepartment);
};
