#include "sort.h"

namespace templates {
    void copy_arr(void** ppArray, int left, int right, void** copyArray) {
            for (int i = left; i < right; ++i) {
                copyArray[i] = ppArray[i];
            }
        }

    void merge(void** ppArray, int left, int middle, int right, CompareSortType pCompareFunc, void** copyArray) { 
        copy_arr(ppArray, left, right, copyArray);
        int i = left;
        int j = middle;
        int k = left;
        while (i < middle && j < right) {
            if (pCompareFunc(copyArray[i], copyArray[j]) < 0) {
                // элемент левого массива меньше элемента правого
                ppArray[k] = copyArray[i];
                ++i;
            } else {
                ppArray[k] = copyArray[j];
                ++j;
            }
            ++k;
        }
        while (j < right) {
            ppArray[k] = copyArray[j];
            ++j;
            ++k;
        }
        while (i < middle) {
            ppArray[k] = copyArray[i];
            ++i;
            ++k;
        }
    }

    void preMergeSort(void** ppArray, int left, int right, CompareSortType pCompareFunc, void** copyArray) {
        if (left + 1 >= right) {
            return;
        }
        int middle = (left + right) / 2;
        preMergeSort(ppArray, left, middle, pCompareFunc, copyArray);
        preMergeSort(ppArray, middle, right, pCompareFunc, copyArray);
        merge(ppArray, left, middle, right, pCompareFunc, copyArray);
    }

    void mergeSort(void** ppArray, int length, CompareSortType pCompareFunc) {
            void** copyArray = new void*[length];
            preMergeSort(ppArray, 0, length, pCompareFunc, copyArray);
            delete copyArray;
    }
}
