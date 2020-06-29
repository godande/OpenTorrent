#include <TorrentConnectionv6.h>
#include <boost/asio.hpp>
#include <ios>
#include <iostream>
#include "AnnouncePacket.h"
#include "ConnectPacket.h"
#include "ResponseAnnouncePacket.h"
#include "TorrentConnectionv6.h"
#include "bencode.h"
#include "bencodeelementadapter.h"
#include "logger.h"
#include "torrentmultiplefileInfo.h"
#include "torrentsinglefileinfo.h"
#include "utilities.h"

int main() {
  Logger::get_instance()->Debug("KEK");
  std::ifstream input_file(
      "/home/prise/CLionProjects/CockTorrent/test-torrent-files/"
      "nsfw.torrent",
      std::ios::binary | std::ifstream::in);
  std::cout << input_file.is_open() << '\n';
  std::string expression{std::istreambuf_iterator<char>{input_file},
                         std::istreambuf_iterator<char>{}};
  auto res = bencode::Decode(expression);
  //  std::cout << bencode::Encode(res) << '\n';
  TorrentMultipleFileInfo s_file_s_info{res};
  std::cout << s_file_s_info.announce() << '\n';

  input_file.close();
  /*input_file.open(
      "/home/prise/CLionProjects/CockTorrent/test-torrent-files/"
      "Gravity-Falls-Season-1.torrent",
      std::ios::binary | std::ios_base::in);
  expression = std::string{std::istreambuf_iterator<char>{input_file},
                           std::istreambuf_iterator<char>{}};
  res = bencode::Decode(expression);

  TorrentMultipleFileInfo s_file_m_info{res};
  std::cout << s_file_m_info.announce();
   */
  boost::asio::io_service io_service;
  boost::asio::ip::udp::socket socket_{io_service};
  boost::asio::ip::udp::resolver resolver(io_service);
  boost::asio::ip::udp::resolver::query query(
      "9.rarbg.me", "2930",
      boost::asio::ip::resolver_query_base::numeric_service);
  for (boost::asio::ip::udp::resolver::iterator i = resolver.resolve(query);
       i != boost::asio::ip::udp::resolver::iterator(); ++i) {
    boost::asio::ip::udp::endpoint end = *i;
    std::cout << end.address() << ' ';
    socket_.connect(end);
    ConnectPacket sendPacket{};
    socket_.send(sendPacket.buffer().data());
    boost::asio::streambuf buf{16};
    socket_.receive(buf.prepare(16));
    buf.commit(16);
    ResponseConnectPacket income(buf, sendPacket.transactionID());
    std::cout << income.transactionID() << " " << income.connectionID() << " "
              << income.action() << '\n';
    std::array<char, 20> generated{};
    std::uniform_int_distribution<char> distribution;
    std::generate(generated.begin(), generated.end(), [&distribution]() {
      return distribution(TorrentConnectionv6::generator);
    });
    std::uniform_int_distribution<uint32_t> distribution2;
    AnnouncePacket sendAnnounce(
        income.connectionID(),
        util::StringToCharArray<20>(s_file_s_info.pieces().substr(0, 20)),
        generated, 0, 0, 0, 2, 0, distribution2(TorrentConnectionv6::generator),
        -1, 1337, 0);
    socket_.send(sendAnnounce.buffer().data());
    boost::asio::streambuf buffer{};
    socket_.receive(buffer.prepare(140));
    buffer.commit(140);
    ResponseAnnouncePacket receivePacket(buffer, sendAnnounce.transactionID());
    std::cout << receivePacket.action() << ' ' << receivePacket.transactionId()
              << ' ' << receivePacket.leechers() << ' '
              << receivePacket.seeders() << ' ' << receivePacket.interval()
              << '\n';
    for(const auto& x : receivePacket.peers()) {
      std::cout << x.ip << ' ' << x.port << '\n';
    }
  }
  return 0;
}
