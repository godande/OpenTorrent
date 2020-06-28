//
// Created by prise on 6/28/20.
//

#ifndef COCKTORRENT_PACKET_H
#define COCKTORRENT_PACKET_H


#include <boost/asio.hpp>

class Packet {
public:

    Packet();
    void EncodeToBinary();
    void DecodeFromBinary();

private:
    static constexpr int64_t connectionID_ = 0x41727101980;
    static constexpr int32_t actionID_ = 0;
    int32_t transaction_;
    boost::asio::streambuf buffer_;
};


#endif //COCKTORRENT_PACKET_H
