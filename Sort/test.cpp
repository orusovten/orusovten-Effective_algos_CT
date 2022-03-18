#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "sort.cpp"
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include "doctest.h"

using namespace templates;

const int MAX_LENGTH = 32;

struct Simple {
    std::string text;
    int number;
    Simple(const std::string& text, int number): text(text), number(number) {}
};

bool operator==(const Simple& left, const Simple& right) {
    return left.text == right.text && left.number == right.number;
}

std::string random_string()
{
    int length = std::rand() % MAX_LENGTH;
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }
    return random_string;
}

int LessCompare(const Simple* left, const Simple* right) {
    int diff = left->text.size() - right->text.size();
    if (diff != 0) {
        return diff;
    }
    for (int i = 0; i < left->text.size(); ++i) {
        diff = (left->text)[i] - (right->text)[i];
        if (diff != 0) {
            return diff;
        }
    }
    return left->number - right->number;
}

int BiggerCompare(const Simple* left, const Simple* right) {
    return -LessCompare(left, right);
}

TEST_CASE("main") {
    SUBCASE("less test") {
        const int n = 7;
        std::string str_array[] = {"aba", "ba", "c", "ca", "aab", "a", "a"};
        int int_array[] = {3, 4, 5, 3, 2, 1, 2};
        Simple** array = new Simple*[n];
        for (int i = 0; i < n; ++i) {
            array[i] = new Simple(str_array[i], int_array[i]);
        }
        mergeSort(array, n, LessCompare);
        for (int i = 0; i < n - 1; ++i) {
            CHECK(LessCompare(array[i], array[i + 1]));
        }
        for (int i = 0; i < n; ++i) {
            delete array[i];
        }
        delete array;
    }
    SUBCASE("bigger test") {
        const int n = 7;
        std::string str_array[] = {"aba", "ba", "c", "ca", "aab", "a", "a"};
        int int_array[] = {3, 4, 5, 3, 2, 1, 2};
        Simple** array = new Simple*[n];
        for (int i = 0; i < n; ++i) {
            array[i] = new Simple(str_array[i], int_array[i]);
        }
        mergeSort(array, n, BiggerCompare);
        for (int i = 0; i < n - 1; ++i) {
            CHECK(BiggerCompare(array[i], array[i + 1]));
        }
        for (int i = 0; i < n; ++i) {
            delete array[i];
        }
        delete array;
    }
    SUBCASE("change nothing test") {
        const int n = 4;
        std::string str_array[] = {"a", "b", "aa", "aa"};
        int int_array[] = {1, 2, 3, 4};
        Simple** array = new Simple*[n];
        for (int i = 0; i < n; ++i) {
            array[i] = new Simple(str_array[i], int_array[i]);
        }
        mergeSort(array, n, LessCompare);
        for (int i = 0; i < n; ++i) {
            CHECK(*array[i] == Simple(str_array[i], int_array[i]));
        }
        for (int i = 0; i < n; ++i) {
            delete array[i];
        }
        delete array;
    }
    SUBCASE("reverse after sort test") {
        const int n = 4;
        std::string str_array[] = {"aa", "aa", "b", "a"};
        int int_array[] = {4, 3, 2, 1};
        Simple** array = new Simple*[n];
        for (int i = 0; i < n; ++i) {
            array[i] = new Simple(str_array[i], int_array[i]);
        }
        mergeSort(array, n, LessCompare);
        for (int i = 0; i < n; ++i) {
            CHECK(*array[n - i - 1] == Simple(str_array[i], int_array[i]));
        }
        for (int i = 0; i < n; ++i) {
            delete array[i];
        }
        delete array;
    }
    SUBCASE("random test") {
        const int MAX_NUMBER = 1000;
        const int n = 1000;
        Simple** array = new Simple*[n];
        for (int i = 0; i < n; ++i) {
            array[i] = new Simple(random_string(), std::rand() % MAX_NUMBER);
        }
        mergeSort(array, n, LessCompare);
        for (int i = 0; i < n - 1; ++i) {
            CHECK(LessCompare(array[i], array[i + 1]));
        }
        for (int i = 0; i < n; ++i) {
            delete array[i];
        }
        delete array;
    }
}
