//
// Created by prise on 6/28/20.
//

#include <iostream>
#include "include/logger.h"
#include "include/udp/responseannouncepacket.h"
#include "include/utilities.h"

namespace cocktorrent::udp {
ResponseAnnouncePacket::ResponseAnnouncePacket(boost::asio::streambuf& buffer,
                                               int32_t transactionID) {
  if (buffer.size() < 20) {
    Logger::get_instance()->Error("Bad packet");
    throw std::runtime_error{"Bad packet"};
  }
  util::CharSequence<4> tempBuf{};
  util::CharSequence<2> portBuf{};

  buffer.sgetn(tempBuf.chars, 4);
  this->action_ = util::FromNetworkCharSequence<int32_t>(tempBuf);

  buffer.sgetn(tempBuf.chars, 4);
  this->transactionID_ = util::FromNetworkCharSequence<int32_t>(tempBuf);

  if (this->transactionID_ != transactionID) {
    Logger::get_instance()->Error("Transaction ID mismatch");
    throw std::runtime_error{"Transaction ID mismatch"};
  }

  buffer.sgetn(tempBuf.chars, 4);
  this->interval_ = util::FromNetworkCharSequence<int32_t>(tempBuf);

  buffer.sgetn(tempBuf.chars, 4);
  this->leechers_ = util::FromNetworkCharSequence<int32_t>(tempBuf);

  buffer.sgetn(tempBuf.chars, 4);
  this->seeders_ = util::FromNetworkCharSequence<int32_t>(tempBuf);

  while (buffer.size() > 0) {
    buffer.sgetn(tempBuf.chars, 4);
    buffer.sgetn(portBuf.chars, 2);
    auto to_return = Seed{util::FromNetworkCharSequence<uint32_t>(tempBuf),
                          util::FromNetworkCharSequence<uint16_t>(portBuf)};
    if (to_return.port || to_return.ip) {
      peers_.push_back(to_return);
      std::cout << util::ToNetworkCharSequence<int32_t>(to_return.ip).chars
                << '\n';
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
}  // namespace cocktorrent::udp