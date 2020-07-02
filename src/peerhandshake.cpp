//
// Created by prise on 7/2/20.
//

#include "include/peerhandshake.h"
#include "include/torrentconnectionv6.h"
#include "include/utilities.h"

namespace cocktorrent::peer::tcp {
PeerHandshake::PeerHandshake(const std::array<char, 20> &info_hash)
    : info_hash_(info_hash) {
  std::uniform_int_distribution<char> rand('0', '9');
  std::generate(peer_id_.begin() + 8, peer_id_.end(),
                [&rand]() { return rand(util::generator); });

  buffer_.sputc(pstrlen);
  buffer_.sputn(protocol_id.data(), protocol_id.size());
  buffer_.sputn(util::ToNetworkCharSequence(PeerHandshake::reserved).chars, 8);
  buffer_.sputn(info_hash_.data(), 20);
  buffer_.sputn(peer_id_.data(), 20);
}
}  // namespace cocktorrent::peer::tcp