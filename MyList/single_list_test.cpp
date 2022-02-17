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
    CSingleLinkedList<Simple> single_list;
    int N_iter = 100;
    for (int i = 0; i < N_iter; ++i) {
        Simple elem(i, std::to_string(i));
        single_list.pushBack(elem);
    }
    CHECK(single_list.getSize() == N_iter);
    for (int i = 0; i < N_iter; ++i) {
        Simple elem(i, std::to_string(i));
        CHECK(elem == single_list.popFront()); 
    }
    CHECK(single_list.getSize() == 0);
}

TEST_CASE("pushFront") {
        CSingleLinkedList<Simple> single_list;
    int N_iter = 100;
    for (int i = N_iter - 1; i >= 0; --i) {
        Simple elem(i, std::to_string(i));
        single_list.pushFront(elem);
    }
    for (int i = 0; i < N_iter; ++i) {
        Simple elem(i, std::to_string(i));
        CHECK(elem == single_list.popFront()); 
    }
}

TEST_CASE("erase") {
    // удаляем (N_iter / 2)-ый элемент, пока не останется (N_iter / 2) элементов, 
    // далее удаляем только первый, пока не останется (N_iter / 4) элементов
    // далее через уже проверенный popFront проверяем, что остались корректные элементы
    CSingleLinkedList<Simple> single_list;
    int N_iter = 100;
    for (int i = 0; i < N_iter; ++i) {
        Simple elem(i, std::to_string(i));
        single_list.pushBack(elem);
    }
    CSingleLinkedList<Simple>::CIterator it = single_list.begin();
    for (int i = 0; i < N_iter / 2; ++i, ++it);
    for (; it.isValid(); ++it) {
        single_list.erase(it);
    }
    it = single_list.begin();
    for (int i = 0; i < N_iter / 4; ++i, ++it) {
        Simple elem(i, std::to_string(i));
        CHECK(elem == *it);
        single_list.erase(it);
    }
    for (int i = N_iter / 4; i < N_iter / 2; ++i) {
        Simple elem(i, std::to_string(i));
        CHECK(elem == single_list.popFront()); 
    }
}

TEST_CASE("clear") {
    CSingleLinkedList<Simple> single_list;
    int N_iter = 100;
    for (int i = 0; i < N_iter; ++i) {
        Simple elem(i, std::to_string(i));
        single_list.pushBack(elem);
    }
    single_list.clear();
    CHECK(single_list.getSize() == 0);
}
