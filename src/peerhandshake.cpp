//
// Created by prise on 7/2/20.
//

#include "include/peerhandshake.h"
#include "include/utilities.h"

namespace opentorrent::peer::tcp {
PeerHandshake::PeerHandshake(const std::array<char, 20> &info_hash)
    : info_hash_(info_hash) {
  std::uniform_int_distribution<char> rand('0', '9');
  std::generate(peer_id_.begin() + 8, peer_id_.end(),
                [&rand]() { return rand(util::generator); });

//  util::Put(buffer_, pstrlen, protocol_id, reserved, info_hash_, peer_id_);
}
}  // namespace opentorrent::peer::tcp
