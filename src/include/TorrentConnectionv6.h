//
// Created by geodesia on 09.06.2020.
//

#ifndef COCKTORRENT_TORRENTCONNECTIONV6_H
#define COCKTORRENT_TORRENTCONNECTIONV6_H

#include <memory>
#include <boost/asio.hpp>
#include <random>

class TorrentConnectionv6 : std::enable_shared_from_this<TorrentConnectionv6> {
public:
    using IOService = boost::asio::io_service;
    using IPAdress = boost::asio::ip::address_v6;
    using EndPoint = boost::asio::ip::udp::endpoint;
    using Socket = boost::asio::ip::udp::socket;
    using Socket_ptr = std::shared_ptr<Socket>;
    using Error = boost::asio::error::basic_errors;
    using ErrorCode = boost::system::error_code;
    using Buffer = boost::asio::streambuf;
    using Timer = boost::asio::deadline_timer;

    explicit TorrentConnectionv6(IOService &s, EndPoint ep) : service_(s), end_point_(ep),
                                                              socket_(s),
                                                              timer_(s) {
        socket_.async_connect(end_point_, [copy = this->shared_from_this()](const ErrorCode &ec) mutable {
            copy->HandShake();
        });
    }

private:
    void HandShake();

    uint32_t SendConnectRequest();

    void ReceiveConnectResponse(uint32_t transaction_id);

    void SendAnnounceRequest();

    void ReceiveAnnounceResponse();

    static std::mt19937 generator;
    IOService &service_;
    EndPoint end_point_;
    Socket socket_;
    Timer timer_;
};

std::mt19937 TorrentConnectionv6::generator{std::random_device{}()};


#endif //COCKTORRENT_TORRENTCONNECTIONV6_H
