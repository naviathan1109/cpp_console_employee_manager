#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Displays numbered console choices and validates the user's selection.
class Menu {
public:
    // Displays the menu and returns a validated numeric option.
    template <typename T>
    int int_getChoice(const std::vector<T>& options) {
        showMenu(options);
        int choice;
        std::string input;
        do {
            std::cout << "Enter your choice: ";
            std::getline(std::cin, input);
            std::stringstream parser(input);
            if (parser >> choice && parser.eof() &&
                choice >= 0 && choice < static_cast<int>(options.size()))
                return choice;
            std::cout << "Invalid choice. Enter a number listed in the menu.\n";
        } while (true);
    }

    // Returns the option selected by its menu index.
    template <typename T>
    T ref_getChoice(const std::vector<T>& options) {
        int index = int_getChoice(options);
        return options[index];
    }

    // Prints numbered options, placing Exit last while preserving its option code.
    template <typename T>
    void showMenu(const std::vector<T>& options) {
        std::cout << "\n================ EMPLOYEE MANAGER ================\n";
        for (size_t i = 1; i < options.size(); ++i)
            std::cout << i << ". " << options[i] << '\n';
        if (!options.empty())
            std::cout << "0. " << options[0] << '\n';
    }

    // Retained as an empty compatibility overload for existing callers.
    void showMenu() const {}
};
