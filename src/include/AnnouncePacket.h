//
// Created by prise on 6/28/20.
//

#ifndef COCKTORRENT_ANNOUNCEPACKET_H
#define COCKTORRENT_ANNOUNCEPACKET_H
#include <iostream>
#include "ResponcePacket.h"
class AnnouncePacket {
public:
    static constexpr int32_t action_ = 1;

    AnnouncePacket(int64_t connectionId, std::string infoHash, std::string peerId, int64_t downloaded,
                   int64_t left, int64_t uploaded, int32_t event, int32_t ipAddress, int32_t key, int32_t numWant,
                   int16_t port);

private:
    int64_t connectionID_;
    std::string infoHash_;
    std::string peerID_;
    int64_t downloaded_;
    int64_t left_;
    int64_t uploaded_;
    int32_t event_;
    int32_t ipAddress_;
    int32_t key_;
    int32_t numWant_;
    int16_t port_;
};


#endif //COCKTORRENT_ANNOUNCEPACKET_H
