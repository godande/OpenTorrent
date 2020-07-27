#ifndef UTILITIES_H
#define UTILITIES_H

#include <logger.h>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>
#include <charconv>
#include <chrono>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <iostream>

namespace cocktorrent::util {
inline ::std::mt19937 generator{::std::random_device{}()};
template <class T>
using EnableIfIntegral =
    ::std::enable_if<::std::is_integral_v<::std::decay_t<T>>>;

inline auto CurrentDate() {
  auto time = ::std::chrono::system_clock::now();
  auto tm = ::std::chrono::system_clock::to_time_t(time);
  return ::std::put_time(::std::localtime(&tm), "%Y-%m-%d %H:%M:%S");
}

inline bool IsUdp(::std::string_view url) { return url.find("udp://") == 0; }

inline ::boost::asio::ip::udp::resolver::results_type GetUDPEndPoints(
    ::std::string_view url, ::boost::asio::io_context &io_context) {
  std::string_view domain;
  std::string_view port;
  if (IsUdp(url)) {
    url.remove_prefix(sizeof("udp://") - 1);
    auto start_port = url.find(":");
    domain = url.substr(0, start_port);
    auto sv = url.begin() + start_port + 1;
    auto it = sv;
    while (std::isdigit(*it)) ++it;
    port = url.substr(start_port + 1, it - sv);
  } else {
    return {};
  }

  ::boost::asio::ip::udp::resolver resolver{io_context};

  return resolver.resolve(domain, port);
}
template <class T, class InputIt>
::std::vector<T> ToVector(InputIt b, InputIt e) {
  return ::std::vector<T>{b, e};
}

template <class T, class Cont>
::std::vector<T> ToVector(Cont &&c) {
  auto b = ::std::begin(::std::forward<Cont>(c));
  auto e = ::std::begin(::std::forward<Cont>(c));
  return ::cocktorrent::util::ToVector<T>(b, e);
}

template <size_t Size>
struct CharSequence {
  char chars[Size];
};

template <class T, typename = EnableIfIntegral<T>>
T HostToNetwork(T x) noexcept {
  return ::boost::endian::native_to_big(x);
}

template <class T, typename = EnableIfIntegral<T>>
T NetworkToHost(T x) noexcept {
  return ::boost::endian::big_to_native(x);
}

template <class T, typename = EnableIfIntegral<T>>
::cocktorrent::util::CharSequence<sizeof(T)> ToNetworkCharSequence(T x) {
  ::cocktorrent::util::CharSequence<sizeof(T)> chars;
  x = ::cocktorrent::util::HostToNetwork(x);
  ::std::memcpy(chars.chars, &x, sizeof(T));
  return chars;
}
template <class T, typename = EnableIfIntegral<T>>
T FromNetworkCharSequence(::cocktorrent::util::CharSequence<sizeof(T)> bytes) {
  T value;
  ::std::memcpy(&value, bytes.chars, sizeof(T));
  return ::cocktorrent::util::NetworkToHost(value);
}

template <class T, typename = EnableIfIntegral<T>>
T FromNetworkCharSequence(::std::string_view bytes) {
  T value;
  if (bytes.size() == sizeof(T))
    throw ::std::logic_error(
        "sizeof(T) must be equals to bytes.size() in "
        "FromNetworkCharSequence(std::string_view).");
  ::std::memcpy(&value, bytes.data(), sizeof(T));
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

inline void Put(::boost::asio::streambuf &buf, ::std::string_view sv) {
  buf.sputn(sv.data(), sv.size());
}

template <class T, typename = EnableIfIntegral<T>>
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

inline void Put(char *buf, ::std::string_view sv) {
  ::std::memcpy(buf, sv.data(), sv.size());
}

template <class T, typename = EnableIfIntegral<T>>
void Put(char *buf, T el) {
  ::std::memcpy(buf, ::cocktorrent::util::ToNetworkCharSequence(el).chars,
                sizeof(el));
}

template <size_t N>
void Put(char *buf, ::std::array<char, N> ar) {
  ::std::memcpy(buf, ar.data(), sizeof(ar));
}

template <class... T>
void Put(const char *buf, T &&... els) {
  [[maybe_unused]] int dummy_arr[sizeof...(T)] = {
      (::cocktorrent::util::Put(buf += sizeof(T), ::std::forward<T>(els)),
       0)...};
}

template <class... T, size_t N>
void Put(const ::std::array<char, N> &buf, T &&... els) {
  constexpr size_t all_size = (sizeof(T) + ...);
  std::cout << all_size;
//  static_assert(N == all_size, "Array size mismatch.");
  ::cocktorrent::util::Put(buf.data(), std::forward<T>(els)...);
}

}  // namespace cocktorrent::util

#endif  // UTILITIES_H
