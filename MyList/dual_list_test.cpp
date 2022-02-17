#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "list.h"
#include "doctest.h"
#include <iostream>
#include <string> 
using namespace lab618;


struct Simple {
    int number;
    std::string text;
    Simple(int number_, const std::string& text_): number(number_), text(text_) {}
};
bool operator== (const Simple& left, const Simple& right) {
    return left.number == right.number && left.text == right.text;
}

TEST_CASE("init and pop_front") {
    CDualLinkedList<Simple> dual_list;
    int N_iter = 100;
    for (int i = 0; i < N_iter; ++i) {
        Simple elem(i, std::to_string(i));
        dual_list.pushBack(elem);
    }
    CHECK(dual_list.getSize() == N_iter);
    for (int i = 0; i < N_iter; ++i) {
        Simple elem(i, std::to_string(i));
        CHECK(elem == dual_list.popFront()); 
    }
    CHECK(dual_list.getSize() == 0);
}

TEST_CASE("pushFront") {
        CDualLinkedList<Simple> dual_list;
    int N_iter = 100;
    for (int i = N_iter - 1; i >= 0; --i) {
        Simple elem(i, std::to_string(i));
        dual_list.pushFront(elem);
    }
    for (int i = 0; i < N_iter; ++i) {
        Simple elem(i, std::to_string(i));
        CHECK(elem == dual_list.popFront()); 
    }
}

TEST_CASE("erase") {
    // удаляем (N_iter / 2)-ый элемент, пока не останется (N_iter / 2) элементов, 
    // далее удаляем только первый, пока не останется (N_iter / 4) элементов
    // далее через уже проверенный popFront проверяем, что остались корректные элементы
    CDualLinkedList<Simple> dual_list;
    int N_iter = 100;
    for (int i = 0; i < N_iter; ++i) {
        Simple elem(i, std::to_string(i));
        dual_list.pushBack(elem);
    }
    CDualLinkedList<Simple>::CIterator it = dual_list.begin();
    for (int i = 0; i < N_iter / 2; ++i, ++it);
    for (; it.isValid(); ++it) {
        dual_list.erase(it);
    }
    it = dual_list.begin();
    for (int i = 0; i < N_iter / 4; ++i, ++it) {
        Simple elem(i, std::to_string(i));
        CHECK(elem == *it);
        dual_list.erase(it);
    }
    for (int i = N_iter / 4; i < N_iter / 2; ++i) {
        Simple elem(i, std::to_string(i));
        CHECK(elem == dual_list.popFront()); 
    }
}

TEST_CASE("clear") {
    CDualLinkedList<Simple> dual_list;
    int N_iter = 100;
    for (int i = 0; i < N_iter; ++i) {
        Simple elem(i, std::to_string(i));
        dual_list.pushBack(elem);
    }
    dual_list.clear();
    CHECK(dual_list.getSize() == 0);
}
