#include <iostream>
#include <functional>
#include <random>


int random_number(int x, int y) {
    std::uniform_int_distribution<int> d(x, y);
    std::random_device random_dev;
    int random_number = d(random_dev);
    return random_number;
}

int roll_and_add(int times, int sides) {
    int result = 0;
    for (int i = 0; i < times; i++) {
        result += random_number(1, sides);
    };
    return result;
}
