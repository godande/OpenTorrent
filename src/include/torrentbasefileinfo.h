//
// Created by Linux Oid on 28.04.2020.
//

#ifndef COCKTORRENT_TORRENTBASEFILEINFO_H
#define COCKTORRENT_TORRENTBASEFILEINFO_H

#include "bencode.h"
#include "bencodeelementadapter.h"
#include <vector>

namespace cocktorrent {
/**
 * @class TorrentBaseFileInfo
 * @brief Класс с общей инфой для торрент файлов
 * @author https://github.com/s3ponia
 */
class TorrentBaseFileInfo {
 public:
  using Dictionary = bencode::BencodeDictionary;
  using String = bencode::BencodeString;
  using Integer = bencode::BencodeInt;
  using BencodeElement = bencode::BencodeElement;
  using BencodeAdapter = bencode::BencodeElementAdapter<const BencodeElement>;
  using AnnounceList = std::vector<std::string>;
  using InfoHashType = std::array<char, 20>;

  TorrentBaseFileInfo() = delete;

  explicit TorrentBaseFileInfo(BencodeElement const &el);

  [[nodiscard]] const String &announce() const {
    return announce_;
  }

  [[nodiscard]] const AnnounceList &announce_list() const {
    return announce_list_;
  }

  [[nodiscard]] const String &pieces() const {
    return pieces_;
  }

  [[nodiscard]] const String &name() const {
    return name_;
  }

  [[nodiscard]] const InfoHashType &info_hash() const {
    return info_hash_;
  }

  [[nodiscard]] Integer piece_length() const {
    return piece_length_;
  }

 private:
  String announce_{};
  AnnounceList announce_list_{};
  String pieces_{};
  String name_{};
  InfoHashType info_hash_{};
  Integer piece_length_{};
};
}  // namespace cocktorrent

#endif  // COCKTORRENT_TORRENTBASEFILEINFO_H
