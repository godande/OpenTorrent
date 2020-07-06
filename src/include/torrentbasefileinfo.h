//
// Created by Linux Oid on 28.04.2020.
//

#ifndef COCKTORRENT_TORRENTBASEFILEINFO_H
#define COCKTORRENT_TORRENTBASEFILEINFO_H

#include "bencode.h"
#include "bencodeelementadapter.h"

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

  TorrentBaseFileInfo() = delete;

  explicit TorrentBaseFileInfo(BencodeElement const &el);

  [[nodiscard]] const String &announce() const;

  [[nodiscard]] const String &pieces() const;

  [[nodiscard]] const String &name() const;

  Integer piece_length() const;

 private:
  String announce_{};
  String pieces_{};
  String name_{};
  Integer piece_length_{};
};
}  // namespace cocktorrent

#endif  // COCKTORRENT_TORRENTBASEFILEINFO_H
