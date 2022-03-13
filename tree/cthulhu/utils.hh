#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

inline std::string to_lower(const std::string_view & data) {
    std::string result{data};
    std::transform(data.begin(), data.end(), result.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return result;
}

inline std::string _join(std::stringstream & ss, const std::string & delimiter) {
    return "";
}

template <typename Arg, typename... Args>
std::string _join(std::stringstream & ss, const std::string & delimiter, const Arg & arg, const Args & ... args) {
    if constexpr (sizeof...(args) > 0) {
        ss << arg << delimiter;
        return _join(ss, delimiter, args...);
    } else {
        ss << arg;
        return ss.str();
    }
}

template <typename... Args>
std::string join(const std::string & delimiter, const Args & ... args) {
    std::stringstream ss;
    return _join(ss, delimiter, args...);
}

template <typename T>
std::optional<T> from_str(const std::string_view & s) {
    std::stringstream ss;
    ss << s;
    T t;
    ss >> t;
    if (ss.fail()) {
        return std::nullopt;}
    return t;
}

template <typename T>
std::string to_string(const T & t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}
