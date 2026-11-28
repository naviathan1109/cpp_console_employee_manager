#pragma once

#include <string>
#include <vector>

/*
   author: anhnh
   date: 19/11/2026
   description: Khai bao cac ham hien thi menu va xac nhan thoat chuong trinh
*/
int getMenuChoice(const std::vector<std::string>& options);
bool confirmExit(const std::string& unsavedFiles = "");
