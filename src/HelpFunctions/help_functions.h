#pragma once
#include <vector>
#include <string>

inline int NormalizeNumber(int number, int left_bound_inclusive, int right_bound_inclusive, bool is_cyclic_range) {
    if (is_cyclic_range) {
        return number < left_bound_inclusive ? right_bound_inclusive : (number > right_bound_inclusive ? left_bound_inclusive : number);
    }
    return number < left_bound_inclusive ? left_bound_inclusive : (number > right_bound_inclusive ? right_bound_inclusive : number);
}

template <typename T>
inline std::vector<std::string> ConvertToStrVector(std::vector<T> numbers) {
    std::vector<std::string> result;
    for (T& number : numbers) {
        std::string num = std::to_string(number);
        num = num.substr(0, 2 + num.find('.'));
        result.push_back(num);
    }
    return result;
}
