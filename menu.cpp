#include "menu.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

/*
   author: anhnh
   date: 19/11/2026
   description: Hien thi danh sach chuc nang va tra ve chi so lua chon hop le
*/
int getMenuChoice(const std::vector<std::string>& options) {
    if (options.empty())
        throw std::invalid_argument("The menu must contain at least one option.");

    std::cout << "\n======= Employee Manager =======\n";
    for (std::size_t index = 1; index < options.size(); ++index)
        std::cout << index << ". " << options[index] << '\n';
    std::cout << "0. " << options[0] << '\n';

    std::string input;
    while (true) {
        std::cout << "Enter an option index: ";
        if (!std::getline(std::cin, input))
            throw std::runtime_error("Input ended while waiting for an option.");

        std::stringstream parser(input);
        int selectedIndex = -1;
        if (parser >> selectedIndex && parser.eof() &&
            selectedIndex >= 0 &&
            selectedIndex < static_cast<int>(options.size()))
            return selectedIndex;

        std::cout << "Invalid option. Enter an index shown in the menu.\n";
    }
}

/*
   author: anhnh
   date: 19/11/2026
   description: Yeu cau nguoi dung xac nhan co muon thoat chuong trinh hay khong
*/
bool confirmExit() {
    std::string answer;
    while (true) {
        std::cout << "Do you want to exit the program? (y/n): ";
        if (!std::getline(std::cin, answer))
            throw std::runtime_error("Input ended while waiting for exit confirmation.");

        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
        std::cout << "Invalid response. Enter y or n.\n";
    }
}
