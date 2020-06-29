//
// Created by prise on 6/28/20.
//
#include "ConnectPacket.h"
#include <utilities.h>
#include "TorrentConnectionv6.h"
#include "bencode.h"
ConnectPacket::ConnectPacket() {
  std::uniform_int_distribution<uint32_t> distribution;
  transactionID_ = distribution(TorrentConnectionv6::generator);
  buffer_.sputn(util::ToNetworkCharSequence(connectionID_).chars,
                sizeof(int64_t));
  buffer_.sputn(util::ToNetworkCharSequence(actionID_).chars, sizeof(int32_t));
  buffer_.sputn(util::ToNetworkCharSequence(transactionID_).chars,
                sizeof(int32_t));
}
int64_t ConnectPacket::connectionID() { return connectionID_; }
int32_t ConnectPacket::actionID() { return actionID_; }
int32_t ConnectPacket::transactionID() const { return transactionID_; }
const boost::asio::streambuf& ConnectPacket::buffer() const { return buffer_; }
