#include <boost/asio.hpp>
#include <ios>
#include <iostream>
#include "bencode.h"
#include "bencodeelementadapter.h"
#include "logger.h"
#include "torrentmultiplefileInfo.h"
#include "torrentsinglefileinfo.h"
#include "AnnouncePacket.h"
int main() {
  Logger::get_instance()->Debug("KEK");
  std::ifstream input_file(
      "/home/prise/CLionProjects/CockTorrent/test-torrent-files/"
      "doctor-who-single-file.torrent",
      std::ios::binary | std::ifstream::in);
  std::cout << input_file.is_open() << '\n';
  std::string expression{std::istreambuf_iterator<char>{input_file},
                         std::istreambuf_iterator<char>{}};
  auto res = bencode::Decode(expression);
  TorrentSingleFileInfo s_file_s_info{res};
  std::cout << s_file_s_info.announce() << '\n';

  input_file.close();
  input_file.open(
      "/home/prise/CLionProjects/CockTorrent/test-torrent-files/"
      "Gravity-Falls-Season-1.torrent",
      std::ios::binary | std::ios_base::in);
  expression = std::string{std::istreambuf_iterator<char>{input_file},
                           std::istreambuf_iterator<char>{}};
  res = bencode::Decode(expression);

  TorrentMultipleFileInfo s_file_m_info{res};
  std::cout << s_file_m_info.announce();

  return 0;
}
