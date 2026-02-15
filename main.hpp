//
// Created by hardy on 2/15/2026.
//

#ifndef LAB2_MAIN_HPP
#define LAB2_MAIN_HPP
#include <iostream>
#include <vector>

using namespace std;

template<typename T> void shuffleArr  (size_t len, T arr[], size_t *cmp);

template<typename T> bool linearSearch          (vector<T> vec, T item, size_t *cmp);
template<typename T> bool binarySearch          (vector<T> arr, T item, size_t *cmp);
template<typename T> bool bogoBogoSearch        (vector<T> vec, T item, size_t *cmp);
template<typename T> bool constLookupRandSearch (vector<T> vec, T item, size_t *cmp);
template<typename T> bool incGenMemRandSearch   (vector<T> vec, T item, size_t *cmp);
template<typename T> bool randomSearch          (vector<T> arr, T item, size_t *cmp);
template<typename T> bool quickSearch           (vector<T> arr, T item, size_t *cmp);

#endif //LAB2_MAIN_HPP