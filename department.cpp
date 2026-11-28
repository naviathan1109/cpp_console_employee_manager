#include "department.h"

#include <iomanip>
#include <sstream>
#include <utility>

/*
author: anhnh
date: 28/11/2026
description: Khoi tao Department voi cac gia tri mac dinh
*/
Department::Department()
    : departmentID(""), departmentName(""), location(""), budget(0.0) {}

/*
author: anhnh
date: 28/11/2026
description: Khoi tao Department tu ma, ten, vi tri va ngan sach
*/
Department::Department(std::string departmentID, std::string departmentName,
                       std::string location, double budget)
    : departmentID(std::move(departmentID)),
      departmentName(std::move(departmentName)),
      location(std::move(location)),
      budget(budget) {}

/*
author: anhnh
date: 28/11/2026
description: Tra ve ma phong ban
*/
std::string Department::getDepartmentID() const { return departmentID; }

/*
author: anhnh
date: 28/11/2026
description: Tra ve ten phong ban
*/
std::string Department::getDepartmentName() const { return departmentName; }

/*
author: anhnh
date: 28/11/2026
description: Tra ve vi tri phong ban
*/
std::string Department::getLocation() const { return location; }

/*
author: anhnh
date: 28/11/2026
description: Cap nhat ma phong ban
*/
void Department::setDepartmentID(const std::string& value) {
    departmentID = value;
}

/*
author: anhnh
date: 28/11/2026
description: Cap nhat ten phong ban
*/
void Department::setDepartmentName(const std::string& value) {
    departmentName = value;
}

/*
author: anhnh
date: 28/11/2026
description: Cap nhat vi tri phong ban
*/
void Department::setLocation(const std::string& value) {
    location = value;
}

/*
author: anhnh
date: 28/11/2026
description: Cap nhat ngan sach phong ban
*/
void Department::setBudget(double value) {
    budget = value;
}


/*
author: anhnh
date: 28/11/2026
description: Tra ve ngan sach cua phong ban
*/
double Department::getBudget() const {
    return budget;
}

/*
author: anhnh
date: 28/11/2026
description: Dinh dang thong tin phong ban thanh chuoi de hien thi
*/
std::string Department::toString() const {
    std::ostringstream output;
    output << std::left
           << std::setw(8) << departmentID.substr(0, 8) << " | "
           << std::setw(30) << departmentName.substr(0, 30) << " | "
           << std::setw(15) << location.substr(0, 15) << " | "
           << std::right << std::fixed << std::setprecision(2)
           << std::setw(15) << budget;
    return output.str();
}
