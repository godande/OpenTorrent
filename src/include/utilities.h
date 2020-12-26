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
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace opentorrent::util {
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
  return ::opentorrent::util::ToVector<T>(b, e);
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
::opentorrent::util::CharSequence<sizeof(T)> ToNetworkCharSequence(T x) {
  ::opentorrent::util::CharSequence<sizeof(T)> chars;
  x = ::opentorrent::util::HostToNetwork(x);
  ::std::memcpy(chars.chars, &x, sizeof(T));
  return chars;
}
template <class T, typename = EnableIfIntegral<T>>
T FromNetworkCharSequence(::opentorrent::util::CharSequence<sizeof(T)> bytes) {
  T value;
  ::std::memcpy(&value, bytes.chars, sizeof(T));
  return ::opentorrent::util::NetworkToHost(value);
}

template <class T, typename = EnableIfIntegral<T>>
T FromNetworkCharSequence(::std::string_view bytes) {
  using namespace std::literals;
  T value;
  if (bytes.size() != sizeof(T))
    throw ::std::logic_error("sizeof(T) {" + std::to_string(sizeof(T)) +
                             "} must be equals to bytes.size() {" +
                             std::to_string(bytes.size()) +
                             "} in "
                             "FromNetworkCharSequence(std::string_view).");
  ::std::memcpy(&value, bytes.data(), sizeof(T));
  return ::opentorrent::util::NetworkToHost(value);
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
  buf.sputn(::opentorrent::util::ToNetworkCharSequence(el).chars, sizeof(el));
}

template <size_t N>
void Put(::boost::asio::streambuf &buf, ::std::array<char, N> ar) {
  buf.sputn(ar.data(), sizeof(ar));
}

template <class... T>
void Put(::boost::asio::streambuf &buf, T &&...els) {
  [[maybe_unused]] int dummy_arr[sizeof...(T)] = {
      (::opentorrent::util::Put(buf, ::std::forward<T>(els)), 0)...};
}

namespace detail {

inline void Put(char *buf, ::std::string_view sv) {
  ::std::memcpy(buf, sv.data(), sv.size());
}

template <class T, typename = EnableIfIntegral<T>>
void Put(char *buf, T el) {
  ::std::memcpy(buf, ::opentorrent::util::ToNetworkCharSequence(el).chars,
                sizeof(el));
}

template <size_t N>
void Put(char *buf, ::std::array<char, N> ar) {
  ::std::memcpy(buf, ar.data(), sizeof(ar));
}

template <class... T>
void Put(char *buf, T &&...els) {
  [[maybe_unused]] int dummy_arr[sizeof...(T)] = {
      (::opentorrent::util::detail::Put(buf, ::std::forward<T>(els)),
       buf += sizeof(T), 0)...};
}
}  // namespace detail

template <class... T, size_t N>
void Put([[maybe_unused]] ::std::array<char, N> &buf,
         [[maybe_unused]] T &&...els) {
  constexpr size_t all_size = (sizeof(T) + ...);
  static_assert(N == all_size, "Array size mismatch.");
  ::opentorrent::util::detail::Put(buf.data(), std::forward<T>(els)...);
}

}  // namespace opentorrent::util

#endif  // UTILITIES_H
