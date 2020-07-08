//
// Created by Linux Oid on 28.04.2020.
//

#include "torrentbasefileinfo.h"
#include <openssl/sha.h>
#include "bencode.h"
#include "utilities.h"

namespace cocktorrent {
TorrentBaseFileInfo::TorrentBaseFileInfo(
    const TorrentBaseFileInfo::BencodeElement &el)
    : announce_(adapt(&el)["announce"].string()),
      pieces_(adapt(&el)["info"]["pieces"].string()),
      name_(adapt(&el)["info"]["name"].string()),
      piece_length_(adapt(&el)["info"]["piece length"].integer()) {
  auto var = bencode::Encode(*adapt(&el)["info"].element());
  auto temp_v = util::ToVector<unsigned char>(var);
  std::array<unsigned char, 20> temp_array;
  SHA1(temp_v.data(), temp_v.size(), temp_array.data());
  std::copy(temp_v.begin(), temp_v.end(), info_hash_.data());
}

const TorrentBaseFileInfo::String &TorrentBaseFileInfo::announce() const {
  return announce_;
}

const TorrentBaseFileInfo::String &TorrentBaseFileInfo::pieces() const {
  return pieces_;
}

const TorrentBaseFileInfo::String &TorrentBaseFileInfo::name() const {
  return name_;
}

const TorrentBaseFileInfo::InfoHashType &TorrentBaseFileInfo::info_hash()
    const {
  return info_hash_;
}

TorrentBaseFileInfo::Integer TorrentBaseFileInfo::piece_length() const {
  return piece_length_;
}
}  // namespace cocktorrent
