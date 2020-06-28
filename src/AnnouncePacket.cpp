//
// Created by prise on 6/28/20.
//

#include "AnnouncePacket.h"

#include <utility>

AnnouncePacket::AnnouncePacket(int64_t connectionId, std::string infoHash, std::string peerId,
                               int64_t downloaded, int64_t left, int64_t uploaded, int32_t event, int32_t ipAddress,
                               int32_t key, int32_t numWant, int16_t port) : connectionID_(connectionId),
                                                                             infoHash_(std::move(infoHash)), peerID_(std::move(peerId)),
                                                                             downloaded_(downloaded), left_(left),
                                                                             uploaded_(uploaded), event_(event),
                                                                             ipAddress_(ipAddress), key_(key),
                                                                             numWant_(numWant), port_(port) {}
