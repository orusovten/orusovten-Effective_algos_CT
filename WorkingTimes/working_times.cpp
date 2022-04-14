#include <iostream>
#include <chrono>
#include <fstream>
#include <cassert>
#include <vector>
#include <random>
#include <algorithm>
#include <string> 
#include <set>
#include "sort.cpp"
#include "avltree.h"
#include "hash.h"

using namespace lab618;
using namespace templates;

const int MAX_LENGTH = 20;
const int MIN_LENGHT = 7;

std::random_device RANDOM_DEVICE;
std::mt19937 GENERATOR(RANDOM_DEVICE());
const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";


int CompareStr(const std::string& left, const std::string& right) {
    int diff = left.size() - right.size();
    if (diff != 0) {
        return diff;
    }
    for (int i = 0; i < left.size(); ++i) {
        diff = left[i] - right[i];
        if (diff != 0) {
            return diff;
        }
    }
    return 0;
}

struct Simple {
    std::string x;
    std::string y;
	bool operator< (const Simple& other) const
    {
		int diff = CompareStr(x, other.x);
		if (diff != 0) {
			return diff < 0;
		}
        return CompareStr(y, other.y) < 0;
    }
    Simple(const std::string& x, const std::string& y): x(x), y(y) {}
	Simple() = default;
};

bool operator==(const Simple& left, const Simple& right) {
    return left.x == right.x && left.y == right.y;
}

unsigned int HashFunc(const Simple* simple) {
    const std::string* str = &(simple->x);
    unsigned int p = 1;
    unsigned int sum = 0;
    for (int i = 0; i < str->size(); ++i) {
        sum += p * (str->at(i) - '0');
        p *= 17;
    }
    str = &(simple->y);
    for (int i = 0; i < str->size(); ++i) {
        sum += p * (str->at(i) - '0');
        p *= 17;
    }
    return sum;
}

int Compare(const Simple* a, const Simple* b) {
    int diff = CompareStr(a->x, b->x);
    if (diff != 0) {
        return diff;
    }
    return CompareStr(a->y, b->y);
}

std::string random_string()
{
    int length = (std::rand() % (MAX_LENGTH - MIN_LENGHT)) + MIN_LENGHT;
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);
    std::string random_string;
    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(GENERATOR)];
    }
    return random_string;
}

std::chrono::milliseconds create_sorted_array(std::vector<Simple>& elements, Simple**& sorted_array) {
	int N = elements.size();
	sorted_array = new Simple*[N];
	for (int i = 0; i < N; ++i) {
		sorted_array[i] = &elements[i];
	}
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	mergeSort(sorted_array, N, Compare);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//std::cout << "succesfully create sorted_array\n";
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds create_avltree(std::vector<Simple>& elements, CAVLTree<Simple, Compare>& tree) {
	int N = elements.size();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < N; ++i) {
 		tree.add(&elements[i]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//std::cout << "succesfully create avl\n";
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds create_hashtable(std::vector<Simple>& elements, CHash<Simple, HashFunc, Compare>& hashTable) {
	int N = elements.size();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < N; ++i) {
		hashTable.add(&elements[i]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//std::cout << "succesfully create hashtable\n";
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

bool bin_search(Simple**& sorted_array, const Simple& elem, int N) {
	int l = 0;
	int r = N - 1;
	int m;
	while (l <= r) {
		m = (l + r) / 2;
		int diff = Compare(&elem, sorted_array[m]);
		if (diff < 0) {
			r = m - 1;
		} else if (diff > 0) {
			l = m + 1;
		} else {
			return true;
		}
	}
	return false;
}

std::chrono::milliseconds find_one_sorted_array(const std::vector<Simple>& elements, Simple**& sorted_array) {
	int N = elements.size();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < N; ++i) {
		bin_search(sorted_array, elements[i], N);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds find_one_avltree(const std::vector<Simple>& elements, CAVLTree<Simple, Compare>& tree) {
	int N = elements.size();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < N; ++i) {
 		tree.find(elements[i]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds find_one_hashtable(const std::vector<Simple>& elements, CHash<Simple, HashFunc, Compare>& hashTable) {
	int N = elements.size();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < N; ++i) {
		hashTable.find(elements[i]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds find_random_sorted_array(const std::vector<Simple>& random_elements, Simple**& sorted_array) {
	int N = random_elements.size();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < N; ++i) {
		bin_search(sorted_array, random_elements[i], N);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds find_random_avltree(const std::vector<Simple>& random_elements, CAVLTree<Simple, Compare>& tree) {
	int N = random_elements.size();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < N; ++i) {
 		tree.find(random_elements[i]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds find_random_hashtable(const std::vector<Simple>& random_elements, CHash<Simple, HashFunc, Compare>& hashTable) {
	int N = random_elements.size();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < N; ++i) {
		hashTable.find(random_elements[i]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}


std::chrono::milliseconds delete_all_sorted_array(Simple**& sorted_array, int N) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	delete[] sorted_array;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds delete_all_avltree(CAVLTree<Simple, Compare>& tree) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	tree.clear();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds delete_all_hashtable(CHash<Simple, HashFunc, Compare>& hashTable) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	hashTable.clear();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds delete_by_one_avltree(const std::vector<Simple>& elements, CAVLTree<Simple, Compare>& tree) {
	int N = elements.size();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < N; ++i) {
		tree.remove(elements[i]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

std::chrono::milliseconds delete_by_one_hashtable(const std::vector<Simple>& elements, CHash<Simple, HashFunc, Compare>& hashTable) {
	int N = elements.size();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < N; ++i) {
		hashTable.remove(elements[i]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}


void generate_objects(std::vector<Simple>& elements, int N) {
	std::set<Simple> set;
	for (int i = 0; i < N; ++i) {
		Simple element;
		do {
			std::string x = random_string();
			std::string y = random_string();
			element = Simple(x, y);
		} while (set.find(element) != set.end());
		set.insert(element);
		elements.push_back(element);
	}
}

void generate_random_objects(int N, std::vector<Simple>& random_elements) {
	for (int i = 0; i < N * 2; ++i) {
		std::string x = random_string();
		std::string y = random_string();
		random_elements.emplace_back(x, y);
	}
}

void working_processes(const std::string& create_all_csv, const std::string& find_one_csv, 
                        const std::string& find_random_csv, const std::string& delete_all_csv,
                         const std::string& delete_by_one_csv) {
	std::ofstream create_all_f("./" + create_all_csv);
	assert(create_all_f.is_open());
	std::ofstream find_one_f("./" + find_one_csv);
	assert(find_one_f.is_open());
	std::ofstream find_random_f("./" + find_random_csv);
	assert(find_random_f.is_open());
	std::ofstream delete_all_f("./" + delete_all_csv);
	assert(delete_all_f.is_open());
	std::ofstream delete_by_one_f("./" + delete_by_one_csv);
	assert(delete_by_one_f.is_open());
	const int mid = 1;
    const int N_start = 10000;
	const int N_end = 1000000;
    const int step = 110000;
	create_all_f << "N,sorted,avltree,hashtable\n";
	find_one_f << "N,sorted,avltree,hashtable\n";
	find_random_f << "N,sorted,avltree,hashtable\n";
	delete_all_f << "N,sorted,avltree,hashtable\n";
	delete_by_one_f << "N,avltree,hashtable\n";
    for (int N = N_start; N <= N_end; N += step) {
		int create_sorted_time = 0;
		int create_hashtable_time = 0;
		int create_avltree_time = 0;

		int find_one_sorted_time = 0;
		int find_one_hashtable_time = 0;
		int find_one_avltree_time = 0;

		int find_random_sorted_time = 0;
		int find_random_hashtable_time = 0;
		int find_random_avltree_time = 0;

		int delete_all_sorted_time = 0;
		int delete_all_hashtable_time = 0;
		int delete_all_avltree_time = 0;

		int delete_by_one_hashtable_time = 0;
		int delete_by_one_avltree_time = 0;

		for (int i = 0; i < mid; ++i) {
			std::vector<Simple> elements;
			generate_objects(elements, N);
			Simple** sorted_array;
			CAVLTree<Simple, Compare> tree;
			CHash<Simple, HashFunc, Compare> hashtable(N, 10000);

			create_avltree_time += create_avltree(elements, tree).count();
			create_hashtable_time += create_hashtable(elements, hashtable).count();
			create_sorted_time += create_sorted_array(elements, sorted_array).count();

			find_one_sorted_time += find_one_sorted_array(elements, sorted_array).count();
			find_one_hashtable_time += find_one_hashtable(elements, hashtable).count();
			find_one_avltree_time += find_one_avltree(elements, tree).count();

			std::vector<Simple> random_elements;
			generate_random_objects(N, random_elements);
			find_random_sorted_time += find_random_sorted_array(elements, sorted_array).count();
			find_random_hashtable_time += find_random_hashtable(elements, hashtable).count();
			find_random_avltree_time += find_random_avltree(elements, tree).count();

			delete_all_sorted_time += delete_all_sorted_array(sorted_array, N).count();
			delete_all_avltree_time += delete_all_avltree(tree).count();
			delete_all_hashtable_time += delete_all_hashtable(hashtable).count();
			
			create_avltree(elements, tree).count();
			create_hashtable(elements, hashtable).count();
			delete_by_one_hashtable_time += delete_by_one_hashtable(elements, hashtable).count();
			delete_by_one_avltree_time += delete_by_one_avltree(elements, tree).count();

		}
		create_all_f << N << "," << (create_sorted_time / mid) << "," <<
			(create_avltree_time / mid) << "," << (create_hashtable_time / mid) << "\n";

		find_one_f << N << "," << (find_one_sorted_time / mid) << "," << 
			(find_one_avltree_time / mid) << "," << (find_one_hashtable_time / mid) << "\n";

		find_random_f << N << "," << (find_random_sorted_time / mid) << "," << 
			(find_random_avltree_time / mid) << "," << (find_random_hashtable_time / mid) << "\n";

		delete_all_f << N << "," << (delete_all_sorted_time / mid) << "," <<
			(delete_all_avltree_time / mid) << "," << (delete_all_hashtable_time / mid) << "\n";

		delete_by_one_f << N << "," << (delete_by_one_avltree_time / mid) << 
			"," << (delete_by_one_hashtable_time / mid) << "\n";
		std::cout << N << " completed" << std::endl;
    }
	create_all_f.close();
	find_one_f.close();
	find_random_f.close();
	delete_all_f.close();
	delete_by_one_f.close();
}

int main() {

	const std::string create_all_csv("create_all.csv");
	const std::string find_one_csv("find_one.csv");
    const std::string find_random_csv("find_random.csv");
    const std::string delete_all_csv("delete_all.csv");
    const std::string delete_by_one_csv("delete_by_one.csv");

    working_processes(create_all_csv, find_one_csv, find_random_csv, delete_all_csv, delete_by_one_csv);

	return 0;
}
