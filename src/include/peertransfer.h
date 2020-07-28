//
// Created by Linux Oid on 27.07.2020.
//

#ifndef COCKTORRENT_PEERTRANSFER_H
#define COCKTORRENT_PEERTRANSFER_H

#include <udp/responseannouncepacket.h>
#include <boost/asio.hpp>
#include <vector>

namespace cocktorrent::peer::tcp {
class PeerTransfer {
 public:
  enum class PeerState {
    INTERESTED = 0,
    CHOCKED = 2,
  };

  class Peer {
   public:
    using UnderlyingType =
        std::make_unsigned_t<std::underlying_type_t<PeerState>>;

    void SetChoked() {
      state |= static_cast<UnderlyingType>(PeerState::CHOCKED);
    }

    void SetInterested() {
      state |= static_cast<UnderlyingType>(PeerState::INTERESTED);
    }

    void UnsetChoked() {
      state &= ~static_cast<UnderlyingType>(PeerState::CHOCKED);
    }

    void UnsetInterested() {
      state &= ~static_cast<UnderlyingType>(PeerState::INTERESTED);
    }

    [[nodiscard]] bool Interested() const {
      return state & static_cast<UnderlyingType>(PeerState::INTERESTED);
    }

    [[nodiscard]] bool Choked() const {
      return state & static_cast<UnderlyingType>(PeerState::CHOCKED);
    }

    [[nodiscard]] bool Unchoked() const { return !Choked(); }

    [[nodiscard]] bool NotInterested() const { return !Interested(); }

    [[nodiscard]] const auto &seed() const { return seed_; }

   private:
    udp::ResponseAnnouncePacket::Seed seed_{};
    unsigned state{};
  };

  using EndPoint = boost::asio::ip::tcp::endpoint;
  using Socket = boost::asio::ip::tcp::socket;
  using EndPoints = boost::asio::ip::tcp::resolver::results_type;
  using PeerSet = std::vector<Peer>;

 private:
  PeerSet peers_;
  Socket socket_;
};  // namespace cocktorrent::peer::tcp
}  // namespace cocktorrent::peer::tcp

#endif  // COCKTORRENT_PEERTRANSFER_H
