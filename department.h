#pragma once

#include <string>

// Stores a department's identity, location, and salary budget.
class Department {
private:
    std::string departmentID;
    std::string departmentName;
    std::string location;
    double budget;

public:
    Department();
    Department(std::string departmentID, std::string departmentName,
               std::string location, double budget);

    std::string getDepartmentID() const;
    std::string getDepartmentName() const;
    std::string getLocation() const;
    double getBudget() const;

    void setDepartmentID(const std::string& departmentID);
    void setDepartmentName(const std::string& departmentName);
    void setLocation(const std::string& location);
    void setBudget(double budget);

    std::string toString() const;
};
