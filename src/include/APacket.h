//
// Created by prise on 6/28/20.
//

#ifndef COCKTORRENT_APACKET_H
#define COCKTORRENT_APACKET_H
#include <iostream>
#include "RPacket.h"
class APacket : public RPacket{
public:
    APacket();
    std::string infoHash;
    std::string peerID;
    int64_t downloaded;
    int64_t left;
    int64_t uploaded;
    int32_t integer;
    int32_t ipAddress;
    int32_t key;
    int32_t numWant;
    int16_t port;
};


#endif //COCKTORRENT_APACKET_H
