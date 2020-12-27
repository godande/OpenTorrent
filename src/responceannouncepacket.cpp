//
// Created by prise on 6/28/20.
//

#include "include/logger.h"
#include "include/udp/responseannouncepacket.h"
#include "include/utilities.h"
enum Offset {
  action = 0,
  transaction = 1,
  interval = 2,
  leechers = 3,
  seeders = 4
};
namespace opentorrent::udp {
ResponseAnnouncePacket::ResponseAnnouncePacket(
    const boost::asio::const_buffer& buffer, int32_t transactionID) {
  if (buffer.size() < 20) {
    Logger::get_instance()->Error("Bad packet");
    throw std::runtime_error{"Bad packet"};
  }
  std::string_view buff_view{static_cast<const char*>(buffer.data()),
                             buffer.size()};
  //  action_ = util::FromNetworkCharSequence<int32_t>(
  //      buff_view.substr(4 * Offset::action, 4));
  //  transactionID_ = util::FromNetworkCharSequence<int32_t>(
  //      buff_view.substr(4 * Offset::transaction, 4));
  //  interval_ = util::FromNetworkCharSequence<int32_t>(
  //      buff_view.substr(4 * Offset::interval, 4));
  //  leechers_ = util::FromNetworkCharSequence<int32_t>(
  //      buff_view.substr(4 * Offset::leechers, 4));
  //  seeders_ = util::FromNetworkCharSequence<int32_t>(
  //      buff_view.substr(4 * Offset::seeders, 4));
  std::tie(action_, transactionID_, interval_, leechers_, seeders_) =
      util::Get<decltype(action_), decltype(transactionID_),
                decltype(interval_), decltype(leechers_), decltype(seeders_)>(
          buff_view);
  if (transactionID_ != transactionID) {
    Logger::get_instance()->Error(
        "ResponseAnnouncePacket: Transaction ID mismatch");
    throw std::runtime_error{"ResponseAnnouncePacket: Transaction ID mismatch"};
  }

  buff_view.remove_prefix(4 * (Offset::seeders + 1));
  while (buff_view.size() >= 6) {
    auto to_return =
        Seed{util::FromNetworkCharSequence<uint32_t>(buff_view.substr(0, 4)),
             util::FromNetworkCharSequence<uint16_t>(buff_view.substr(4, 2))};
    buff_view.remove_prefix(6);
    if (to_return.port || to_return.ip) {
      peers_.push_back(to_return);
    }
  }
}

int32_t ResponseAnnouncePacket::action() const { return action_; }

int32_t ResponseAnnouncePacket::transactionId() const { return transactionID_; }

int32_t ResponseAnnouncePacket::interval() const { return interval_; }

int32_t ResponseAnnouncePacket::leechers() const { return leechers_; }

int32_t ResponseAnnouncePacket::seeders() const { return seeders_; }

const std::vector<ResponseAnnouncePacket::Seed>& ResponseAnnouncePacket::peers()
    const {
  return peers_;
}
}  // namespace opentorrent::udp
