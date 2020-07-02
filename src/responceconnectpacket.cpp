//
// Created by prise on 6/28/20.
//

#include "include/logger.h"
#include "include/udp/responseconnectpacket.h"
#include "include/utilities.h"

namespace cocktorrent::udp {

int32_t ResponseConnectPacket::action() const { return action_; }
int32_t ResponseConnectPacket::transactionID() const { return transactionID_; }
int64_t ResponseConnectPacket::connectionID() const { return connectionID_; }
ResponseConnectPacket::ResponseConnectPacket(boost::asio::streambuf& buf,
                                             int32_t transactionID) {
  util::CharSequence<4> tempBuf{};
  util::CharSequence<8> portBuf{};

  buf.sgetn(tempBuf.chars, 4);
  this->action_ = util::FromNetworkCharSequence<int32_t>(tempBuf);

  buf.sgetn(tempBuf.chars, 4);
  this->transactionID_ = util::FromNetworkCharSequence<int32_t>(tempBuf);

  buf.sgetn(portBuf.chars, 8);
  this->connectionID_ = util::FromNetworkCharSequence<int64_t>(portBuf);

  if (transactionID_ != transactionID) {
    Logger::get_instance()->Error("TransactionID mismatch");
    throw std::logic_error{"TransactionID mismatch"};
  }
  if (action_ != 0) {
    Logger::get_instance()->Error("Action mismatch");
    throw std::logic_error{"Action mismatch"};
  }
}
}  // namespace cocktorrent::udp