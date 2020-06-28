//
// Created by prise on 6/28/20.
//

#ifndef COCKTORRENT_RESPONCEPACKET_H
#define COCKTORRENT_RESPONCEPACKET_H
#include <cinttypes>

class ResponcePacket {
public:
     int32_t action;
     int32_t transactionID;
     int64_t connectionID;
private:

};


#endif //COCKTORRENT_RESPONCEPACKET_H
