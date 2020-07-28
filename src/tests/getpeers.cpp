#include <udp/trackerconnection.h>
#include <boost/asio.hpp>
#include <ios>
#include <iostream>
#include <string>
#include "../third-party/include/catch.hpp"
#include "bencode.h"
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
  boost::asio::io_context io_service;
  boost::asio::ip::udp::socket socket_{io_service};
  ::boost::system::error_code ec;
  auto endpoints =
      util::GetUDPEndPoints(ec, s_file_s_info.announce(), io_service);
  if (ec) {
    WARN("Error in getting udp points to " + s_file_s_info.announce() + " " +
         ec.message());
  }
  auto an_list_it = s_file_s_info.announce_list().begin();
  std::size_t peer_size{};
  while (endpoints.empty() &&
         an_list_it != s_file_s_info.announce_list().end()) {
    if (util::IsUdp(*an_list_it)) {
      endpoints = util::GetUDPEndPoints(ec, *an_list_it, io_service);
      if (ec) {
        WARN("Error in getting udp points to " + *an_list_it + " " +
             ec.message());
      }
    }
    ++an_list_it;
  }

  if (!endpoints.empty()) {
    TrackerConnection::DeadLineTimer deadline{io_service};
    deadline.expires_from_now(boost::asio::chrono::seconds(120));
    TrackerConnection tracker_connection{io_service, s_file_s_info};
    deadline.async_wait(
        [&]([[maybe_unused]] const TrackerConnection::ErrorCode &ec) {
          WARN(ec.message());
          io_service.stop();
          peer_size += tracker_connection.peers().size();
        });
    tracker_connection.Run(endpoints);

    io_service.run();
    REQUIRE(peer_size > 0);
  }
}
