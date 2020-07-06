//
// Created by prise on 6/28/20.
//

#ifndef COCKTORRENT_RESPONSEANNOUNCEPACKET_H
#define COCKTORRENT_RESPONSEANNOUNCEPACKET_H

#include <boost/asio.hpp>
#include <cinttypes>
#include <vector>

namespace cocktorrent::udp {
class ResponseAnnouncePacket {
 public:
  ResponseAnnouncePacket() = delete;
  explicit ResponseAnnouncePacket(boost::asio::streambuf& buffer,
                                  int32_t transactionID);
  struct Seed {
    uint32_t ip;
    uint16_t port;
  };

  [[nodiscard]] int32_t action() const;

  [[nodiscard]] int32_t transactionId() const;

  [[nodiscard]] int32_t interval() const;

  [[nodiscard]] int32_t leechers() const;

  [[nodiscard]] int32_t seeders() const;

  [[nodiscard]] const std::vector<Seed>& peers() const;

 private:
  int32_t action_{};
  int32_t transactionID_{};
  int32_t interval_{};
  int32_t leechers_{};
  int32_t seeders_{};
  std::vector<Seed> peers_;
};
}  // namespace cocktorrent::udp
#endif  // COCKTORRENT_RESPONSEANNOUNCEPACKET_H
