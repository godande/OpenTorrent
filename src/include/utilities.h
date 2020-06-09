#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <boost/endian/conversion.hpp>

namespace util {
    inline auto CurrentDate() {
        auto time = std::chrono::system_clock::now();
        auto tm = std::chrono::system_clock::to_time_t(time);
        return std::put_time(std::localtime(&tm), "%Y-%m-%d %H:%M:%S");
    }

    template<size_t Size>
    struct CharSequence {
        char chars[Size];
    };

    template<class T>
    T HostToNetwork(T x) noexcept {
        return boost::endian::native_to_big(x);
    }

    template<class T>
    T NetworkToHost(T x) noexcept {
        return boost::endian::big_to_native(x);
    }

    template<class T>
    const CharSequence<sizeof(T)> ToNetworkCharSequence(T x) {
        union {
            CharSequence<sizeof(T)> chars;
            T value;
        } converter;
        converter.value = HostToNetwork(x);
        return converter.chars;
    }
}

#endif // UTILITIES_H