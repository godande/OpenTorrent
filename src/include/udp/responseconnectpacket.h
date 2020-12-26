//
// Created by prise on 6/28/20.
//

#ifndef COCKTORRENT_RESPONSECONNECTPACKET_H
#define COCKTORRENT_RESPONSECONNECTPACKET_H
#include <boost/asio.hpp>
#include <cinttypes>

namespace opentorrent::udp {
class ResponseConnectPacket {
 public:
  ResponseConnectPacket(const boost::asio::const_buffer &buf,
                        int32_t transactionID);
  [[nodiscard]] int32_t action() const;
  [[nodiscard]] int32_t transactionID() const;
  [[nodiscard]] int64_t connectionID() const;

 private:
  int32_t action_;
  int32_t transactionID_{};
  int64_t connectionID_{};
};
}  // namespace opentorrent::udp

#endif  // COCKTORRENT_RESPONSECONNECTPACKET_H
