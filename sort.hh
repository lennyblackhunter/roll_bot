#ifndef __USER_SORT
#define __USER_SORT
#include<utility>
#include <random>
#include <tuple>
#include <iostream>

template<typename T>
void print_vec(const std::vector<T> & tab) {
    if (tab.empty()) {
        std::cout << "[]" << std::endl;
    } else {
        std::cout << "[" << tab[0];
        for (int i = 1; i < tab.size(); ++i) {
            std::cout << ", " << tab[i];
        }
        std::cout << "]" << std::endl;
    }
}

template<typename T>
void sort(std::vector<T> & tab) {
    for (int i = 0; i < tab.size(); i++) {
        T x = tab[i];
        int y = i;
        for (int j = i; j < tab.size(); j++) {
            if (x <= tab[j]) {
                x = tab[j];
                y = j;
            }
        }
        tab[y] = tab[i];
        tab[i] = x;
    }    
}

//(a, b, c) -> (b, c, a)
template<typename T>
void swap(T & a, T & b, T & c) {
    T x;
    x = a;
    a = b;
    b = c;
    c = x;
}

template<typename T>
std::tuple<int, int> quicksort_pivot(std::vector<T> & tab, int begin, int end) {
    std::random_device r;
    std::default_random_engine eng(r());
    std::uniform_int_distribution<> distrib(begin, end - 1);
    int pivot = distrib(eng);
    // std::cout << "pivot: " << pivot << std::endl;
    // std::cout << "pivot value: " << tab[pivot] << std::endl;
    int m = 0;
    int d = 0;
    std::swap(tab[begin], tab[pivot]);
    for (int i = begin; i < end - 1; i++) {
        if (tab[i + 1] > tab[i]) {
            std::swap(tab[i + 1], tab[begin + m + d]);
            d++;
        }
        else if (tab[i + 1] < tab[i]) {
            swap(tab[i + 1], tab[begin + m + d], tab[begin + m]);
            m++;
        }
    }
    return std::make_tuple(m, d);
}

template<typename T>
void subtab_swap(std::vector<T> & tab, int begin1, int begin2, int end) {
    int x = end - begin2;
    while (begin2 - begin1 > x) {
        for (int i = begin2 - x; i < begin2; i++) {
            std::swap(tab[i], tab[i + x]);
        }
        begin2 = begin2 - x;
    }
    // std::cout << "begin2: " << begin2 << ", x: " << x << std::endl;
    for (int i = begin1; i < begin2; i++) {
        std::swap(tab[i], tab[i + x]);
    }
}

template<typename T>
void quicksort(std::vector<T> & tab, int begin, int end) {
    if (end - begin < 2) {
        return;
    }
    auto [m, d] = quicksort_pivot(tab, begin, end);
    quicksort(tab, begin, begin + m);
    quicksort(tab, begin + m, begin + m + d);
    subtab_swap(tab, begin + m, begin + m + d, end);
}

template<typename T>
T user_max(const std::vector<T> & tab) {
    T x = tab[0];
    for (T i: tab) {
        if (x <= i) {
            x = i;
        }
    }
    return x;
}

#endif // USER_SORT