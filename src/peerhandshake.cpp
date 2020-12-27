//
// Created by prise on 7/2/20.
//

#include "include/peerhandshake.h"
#include "include/utilities.h"

#define PEER_ID_SIZE 8
#define PROTO_BUFFER_SIZE 20
namespace opentorrent::peer::tcp {
PeerHandshake::PeerHandshake(const std::array<char, 20> &info_hash)
    : info_hash_(info_hash) {
  std::uniform_int_distribution<char> rand('0', '9');
  std::generate(peer_id_.begin() + PEER_ID_SIZE, peer_id_.end(),
                [&rand]() { return rand(util::generator); });
  util::Put(buffer_, pstrlen, util::StringToCharArray<pstrlen>(protocol_id),
            reserved, info_hash_, peer_id_);
}
}  // namespace opentorrent::peer::tcp
