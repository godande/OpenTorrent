//
// Created by prise on 6/28/20.
//

#include "ResponseAnnouncePacket.h"
#include "logger.h"
#include "utilities.h"

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
    peers_.emplace_back(Seed{util::FromNetworkCharSequence<int32_t>(tempBuf),util::FromNetworkCharSequence<uint16_t>(portBuf)});
  }
}

int32_t ResponseAnnouncePacket::action() const { return action_; }

int32_t ResponseAnnouncePacket::transactionId() const { return transactionID_; }

int32_t ResponseAnnouncePacket::interval() const { return interval_; }

int32_t ResponseAnnouncePacket::leechers() const { return leechers_; }

int32_t ResponseAnnouncePacket::seeders() const { return seeders_; }

const std::vector<ResponseAnnouncePacket::Seed> &ResponseAnnouncePacket::peers()
    const {
  return peers_;
}
