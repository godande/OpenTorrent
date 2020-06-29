//
// Created by prise on 6/28/20.
//

#ifndef COCKTORRENT_CONNECTPACKET_H
#define COCKTORRENT_CONNECTPACKET_H

#include <boost/asio.hpp>

class ConnectPacket {
 public:
  ConnectPacket();
  static int64_t connectionID();
  static int32_t actionID();
  [[nodiscard]] int32_t transactionID() const;
  [[nodiscard]] const boost::asio::streambuf& buffer() const;

 private:
  static constexpr int64_t connectionID_ = 0x41727101980;
  static constexpr int32_t actionID_ = 0;
  int32_t transactionID_;
  boost::asio::streambuf buffer_;
};

#endif  // COCKTORRENT_CONNECTPACKET_H
