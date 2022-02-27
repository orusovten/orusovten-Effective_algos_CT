#include <iostream>
#include <string>
#include <cstring>
#include "mm.h"
using namespace lab618;

struct Simple{
    int number;
    std::string text = "Tengis";
    Simple() = default;
    Simple(int _number, std::string _text): number(_number), text(_text) {}
};
bool operator==(const Simple& left, const Simple& right) {
    return left.number == right.number && left.text == right.text;
}
std::ostream& operator<<(std::ostream& out, const Simple& simple) {
    out << simple.number << " " << simple.text << std::endl;
    return out;
}

int main() {
    CMemoryManager<Simple> mm(3, false);
    auto p = mm.newObject();
    std::cout << mm.deleteObject(p) << std::endl;
}
