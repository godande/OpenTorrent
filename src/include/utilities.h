#ifndef UTILITIES_H
#define UTILITIES_H

#include <logger.h>
#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>
#include <chrono>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <random>
#include <string>
#include <string_view>

namespace cocktorrent::util {
inline static ::std::mt19937 generator{::std::random_device{}()};

inline auto CurrentDate() {
  auto time = ::std::chrono::system_clock::now();
  auto tm = ::std::chrono::system_clock::to_time_t(time);
  return ::std::put_time(::std::localtime(&tm), "%Y-%m-%d %H:%M:%S");
}

template <class T, class InputIt>
::std::vector<T> ToVector(InputIt b, InputIt e) {
  return ::std::vector<T>{b, e};
}

template<class T, class Cont>
::std::vector<T> ToVector(Cont&& c) {
  auto b = ::std::begin(::std::forward<Cont>(c));
  auto e = ::std::begin(::std::forward<Cont>(c));
  return ::cocktorrent::util::ToVector<T>(b, e);
}

template <size_t Size>
struct CharSequence {
  char chars[Size];
};

template <class T>
T HostToNetwork(T x) noexcept {
  return ::boost::endian::native_to_big(x);
}

template <class T>
T NetworkToHost(T x) noexcept {
  return ::boost::endian::big_to_native(x);
}

template <class T>
::cocktorrent::util::CharSequence<sizeof(T)> ToNetworkCharSequence(T x) {
  ::cocktorrent::util::CharSequence<sizeof(T)> chars;
  x = ::cocktorrent::util::HostToNetwork(x);
  ::std::memcpy(chars.chars, &x, sizeof(T));
  return chars;
}
template <class T>
T FromNetworkCharSequence(::cocktorrent::util::CharSequence<sizeof(T)> bytes) {
  T value;
  ::std::memcpy(&value, bytes.chars, sizeof(T));
  return ::cocktorrent::util::NetworkToHost(value);
}

template <size_t size>
::std::array<char, size> StringToCharArray(::std::string_view symbols) {
  if (symbols.size() != size) {
    Logger::get_instance()->Error("Size mismatch");
    throw ::std::runtime_error{"Size mismatch"};
  }
  ::std::array<char, size> to_return{};
  ::std::copy(symbols.begin(), symbols.end(), to_return.begin());
  return to_return;
}

template <class T>
void Put(::boost::asio::streambuf &buf, T el) {
  buf.sputn(::cocktorrent::util::ToNetworkCharSequence(el).chars, sizeof(el));
}

template <size_t N>
void Put(::boost::asio::streambuf &buf, ::std::array<char, N> ar) {
  buf.sputn(ar.data(), sizeof(ar));
}

template <class... T>
void Put(::boost::asio::streambuf &buf, T &&... els) {
  [[maybe_unused]] int dummy_arr[sizeof...(T)] = {
      (::cocktorrent::util::Put(buf, ::std::forward<T>(els)), 0)...};
}

}  // namespace cocktorrent::util

#endif  // UTILITIES_H
