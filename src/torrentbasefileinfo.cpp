//
// Created by Linux Oid on 28.04.2020.
//

#include "torrentbasefileinfo.h"
#include <openssl/sha.h>
#include <variant>
#include "bencode.h"
#include "utilities.h"

namespace opentorrent {
TorrentBaseFileInfo::TorrentBaseFileInfo(
    const TorrentBaseFileInfo::BencodeElement &el)
    : announce_(adapt(&el)["announce"].string()),
      pieces_(adapt(&el)["info"]["pieces"].string()),
      name_(adapt(&el)["info"]["name"].string()),
      piece_length_(adapt(&el)["info"]["piece length"].integer()) {
  auto var = bencode::Encode(*adapt(&el)["info"].element());
  auto temp_v = util::ToVector<unsigned char>(var);
  std::array<unsigned char, 20> temp_array{};
  SHA1(temp_v.data(), temp_v.size(), temp_array.data());
  std::copy(temp_v.begin(), temp_v.end(), info_hash_.data());

  auto adapted = adapt(&el).dictionary();
  auto it = adapted.find("announce-list");
  if (it == adapted.end()) {
    return;
  }
  auto list = std::get_if<bencode::BencodeList>(&it->second.data);
  if (list == nullptr) {
    return;
  }
  std::for_each(list->begin(), list->end(), [&](auto &&el) {
    auto sublist = std::get_if<bencode::BencodeList>(&el.data);
    if (sublist == nullptr) {
      return;
    }
    std::for_each(sublist->begin(), sublist->end(), [&](auto &&el) {
      if (auto &&el_str = std::get_if<bencode::BencodeString>(&el.data))
        announce_list_.push_back(std::forward<decltype(*el_str)>(*el_str));
    });
  });
}
}  // namespace opentorrent
