#include <udp/trackerconnection.h>
#include <boost/asio.hpp>
#include <ios>
#include <iostream>
#include <string>
#include "../third-party/include/catch.hpp"
#include "bencode.h"
#include "logger.h"
#include "torrentmultiplefileinfo.h"
#include "utilities.h"

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

TEST_CASE("Get peers", "[torrent][tracker]") {
  using namespace cocktorrent;
  using namespace cocktorrent::udp;
  std::ifstream input_file(STRINGIFY(TEST_TORRENT_FILES_PATH) "/nsfw2.torrent",
                           std::ios::binary | std::ifstream::in);
  std::string expression{std::istreambuf_iterator<char>{input_file},
                         std::istreambuf_iterator<char>{}};
  auto res = bencode::Decode(expression);
  TorrentMultipleFileInfo s_file_s_info{res};
  input_file.close();
  boost::asio::io_service io_service;
  boost::asio::ip::udp::socket socket_{io_service};
  auto endpoints = util::GetUDPEndPoints(s_file_s_info.announce(), io_service);
  auto an_list_it = s_file_s_info.announce_list().begin();
  while (endpoints.empty() &&
         an_list_it != s_file_s_info.announce_list().end()) {
    if (util::IsUdp(*an_list_it))
      endpoints = util::GetUDPEndPoints(*an_list_it, io_service);
    ++an_list_it;
  }
  std::intmax_t peer_size{};

  TrackerConnection::DeadLineTimer deadline{io_service};
  deadline.expires_from_now(boost::asio::chrono::seconds(20));
  TrackerConnection tracker_connection{io_service, s_file_s_info.info_hash()};
  deadline.async_wait(
      [&]([[maybe_unused]] const TrackerConnection::ErrorCode &ec) {
        io_service.stop();
        auto &&p = tracker_connection.peers();
        peer_size += p.size();
        std::for_each(std::begin(p), std::end(p), [](auto &&peer) {
          std::cout << peer.ip << ':' << peer.port << '\n';
        });
      });
  tracker_connection.Run(endpoints);

  io_service.run();

  REQUIRE(peer_size > 0);
}
