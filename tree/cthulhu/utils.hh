#pragma once
#include <string>
#include <sstream>


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
