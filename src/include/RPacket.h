//
// Created by prise on 6/28/20.
//

#ifndef COCKTORRENT_RPACKET_H
#define COCKTORRENT_RPACKET_H
#include "iostream"

class RPacket {
public:
     int32_t action;
     int32_t transactionID;
     int64_t connectionID;
private:

};


#endif //COCKTORRENT_RPACKET_H
