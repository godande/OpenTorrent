//
// Created by prise on 6/28/20.
//
#include "include/udp/connectpacket.h"
#include "include/bencode.h"
#include "include/utilities.h"
#include "utilities.h"

namespace cocktorrent::udp {
ConnectPacket::ConnectPacket() {
  std::uniform_int_distribution<uint32_t> distribution;
  transactionID_ = distribution(util::generator);
  util::Put(buffer_, connectionID_, actionID_, transactionID_);
}
int64_t ConnectPacket::connectionID() { return connectionID_; }
int32_t ConnectPacket::actionID() { return actionID_; }
int32_t ConnectPacket::transactionID() const { return transactionID_; }
const ConnectPacket::BufferType& ConnectPacket::buffer() const {
  return buffer_;
}
}  // namespace cocktorrent::udp
