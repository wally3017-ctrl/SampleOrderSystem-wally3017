#pragma once
#include <iostream>
#include <string>

class ConsoleView {
    std::ostream& out_;
    std::istream& in_;

public:
    ConsoleView();
    ConsoleView(std::ostream& out, std::istream& in);

    void Print(const std::string& text);
    void PrintLine(const std::string& text = "");
    void PrintError(const std::string& text);
    void PrintSeparator();
    std::string ReadLine();
};
