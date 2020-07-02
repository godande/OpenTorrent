#ifndef UTILITIES_H
#define UTILITIES_H

#include <logger.h>
#include <stdio.h>
#include <boost/endian/conversion.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>

namespace util {
inline auto CurrentDate() {
  auto time = std::chrono::system_clock::now();
  auto tm = std::chrono::system_clock::to_time_t(time);
  return std::put_time(std::localtime(&tm), "%Y-%m-%d %H:%M:%S");
}

template <size_t Size>
struct CharSequence {
  char chars[Size];
};

template <class T>
T HostToNetwork(T x) noexcept {
  return boost::endian::native_to_big(x);
}

template <class T>
T NetworkToHost(T x) noexcept {
  return boost::endian::big_to_native(x);
}

template <class T>
CharSequence<sizeof(T)> ToNetworkCharSequence(T x) {
  union {
    CharSequence<sizeof(T)> chars;
    T value;
  } converter;
  converter.value = HostToNetwork(x);
  return converter.chars;
}
template <class T>
T FromNetworkCharSequence(CharSequence<sizeof(T)> bytes) {
  union {
    CharSequence<sizeof(T)> chars;
    T value;
  } converter;
  converter.chars = bytes;
  return NetworkToHost(converter.value);
}

template <size_t size>
std::array<char, size> StringToCharArray(std::string_view symbols) {
  if (symbols.size() != size) {
    Logger::get_instance()->Error("Size mismatch");
    throw std::runtime_error{"Size mismatch"};
  }
  std::array<char, size> to_return{};
  std::copy(symbols.begin(), symbols.end(), to_return.begin());
  return to_return;
}

}  // namespace util

#endif  // UTILITIES_H