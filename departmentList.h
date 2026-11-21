#pragma once

#include "department.h"
#include "entityList.h"

#include <string>
#include <vector>

class EmployeeList;

// Manages department records and coordinates department changes with employees.
class DepartmentList : public EntityList<Department> {
private:
    std::vector<Department>& departments();
    const std::vector<Department>& departments() const;

public:
    // Creates an empty department list.
    DepartmentList();

    // Loads pipe-delimited department records and rejects malformed or duplicate IDs.
    bool loadFromFile(const std::string& filename) override;
    // Writes all departments to disk and clears the unsaved-changes flag on success.
    bool saveToFile(const std::string& filename) override;
    // Finds a department by ID and returns its index or -1.
    int searchID(const std::string& id) override;

    // Returns a department at an index, or an empty value for an invalid index.
    Department getDepartment(int index) const;
    // Displays departments and prompts for a valid department ID.
    Department getUserChoice();
    // Returns a copy of all department records.
    std::vector<Department> getDepartments() const;
    // Collects and appends a department with a unique ID.
    void addDepartment();
    // Shows proposed changes and updates a department after confirmation.
    void updateDepartment(EmployeeList& employeeList);
    // Removes a department after resolving its employee assignments.
    bool removeDepartment(const std::string& id, EmployeeList& employeeList);
    // Prints every department or an empty-list message.
    void listDepartments() const;
};
