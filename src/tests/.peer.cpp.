#include "../third-party/include/catch.hpp"
#include <boost/asio.hpp>
#include <ios>
#include "bencode.h"
#include "bencodeelementadapter.h"
#include "logger.h"
#include "torrentmultiplefileinfo.h"
#include "torrentsinglefileinfo.h"
#include "udp/announcepacket.h"
#include "udp/connectpacket.h"
#include "udp/responseconnectpacket.h"
#include "udp/responseannouncepacket.h"
#include "utilities.h"

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

TEST_CASE("Get peers", "[tracker]") {
  using namespace cocktorrent;
  using namespace cocktorrent::udp;
  std::ifstream input_file(
      STRINGIFY(TEST_TORRENT_FILES_PATH)"/nsfw2.torrent",
      std::ios::binary | std::ifstream::in);
  std::string expression{std::istreambuf_iterator<char>{input_file},
                         std::istreambuf_iterator<char>{}};
  auto res = bencode::Decode(expression);
  TorrentMultipleFileInfo s_file_s_info{res};
  input_file.close();
  boost::asio::io_service io_service;
  boost::asio::ip::udp::socket socket_{io_service};
  boost::asio::ip::udp::resolver resolver(io_service);
  boost::asio::ip::udp::resolver::query query(
      "9.rarbg.me", "2930",
      boost::asio::ip::resolver_query_base::numeric_service);
  std::intmax_t peer_size{};
  for (auto i = resolver.resolve(query);
       i != boost::asio::ip::udp::resolver::iterator(); ++i) {
    i = resolver.resolve(query);
    boost::asio::ip::udp::endpoint end = *i;
    socket_.connect(end);
    ConnectPacket sendPacket{};
    socket_.send(sendPacket.buffer().data());
    boost::asio::streambuf buf{16};
    socket_.receive(buf.prepare(16));
    buf.commit(16);
    ResponseConnectPacket income(buf, sendPacket.transactionID());

    std::array<char, 20> generated{};
    std::uniform_int_distribution<char> distribution;
    std::generate(generated.begin(), generated.end(),
                  [&distribution]() { return distribution(util::generator); });
    std::uniform_int_distribution<uint32_t> distribution2;
    AnnouncePacket sendAnnounce(
        income.connectionID(), s_file_s_info.info_hash(), generated, 0, 0, 0,
        2, 0, distribution2(util::generator), -1, 1337, 0);

    socket_.send(sendAnnounce.buffer().data());
    boost::asio::streambuf buffer{};
    socket_.receive(buffer.prepare(140));
    buffer.commit(140);
    ResponseAnnouncePacket receivePacket(buffer, sendAnnounce.transactionID());
    peer_size += receivePacket.peers().size();
    if (peer_size > 0) {
      break;
    }
  }
  REQUIRE(peer_size > 0);
}
