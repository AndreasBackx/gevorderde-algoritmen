
#ifndef PRETTYPRINT_H
#define PRETTYPRINT_H

#include <iostream>
#include <iomanip>
#include <string>

class pretty_print
{
public:

    static void printLine(std::string title = "", char fill_character = '-', int column_width = 80, std::ostream& out = std::cout);
};

void pretty_print::printLine(std::string title, char fill_character, int column_width, std::ostream& out)
{
    if (title.empty())
    {
        std::cout << std::left << std::setw(column_width) << std::setfill(fill_character) << fill_character << std::endl;
    }
    else
    {
        std::cout << std::left << std::setw(column_width) << std::setfill(fill_character) << " " << title << std::endl;
    }
}

#endif