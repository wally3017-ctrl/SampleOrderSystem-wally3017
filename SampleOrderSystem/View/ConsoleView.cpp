#include "ConsoleView.h"

ConsoleView::ConsoleView()
    : out_(std::cout), in_(std::cin) {}

ConsoleView::ConsoleView(std::ostream& out, std::istream& in)
    : out_(out), in_(in) {}

void ConsoleView::Print(const std::string& text) {
    out_ << text;
}

void ConsoleView::PrintLine(const std::string& text) {
    out_ << text << "\n";
}

void ConsoleView::PrintError(const std::string& text) {
    out_ << "[오류] " << text << "\n";
}

void ConsoleView::PrintSeparator() {
    out_ << "============================================================\n";
}

std::string ConsoleView::ReadLine() {
    std::string line;
    std::getline(in_, line);
    if (!line.empty() && line.back() == '\r')
        line.pop_back();
    return line;
}
