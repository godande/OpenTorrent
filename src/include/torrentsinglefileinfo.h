//
// Created by Linux Oid on 24.04.2020.
//

#ifndef COCKTORRENT_TORRENTSINGLEFILEINFO_H
#define COCKTORRENT_TORRENTSINGLEFILEINFO_H

#include "bencode.h"
#include "bencodeelementadapter.h"
#include "torrentbasefileinfo.h"

namespace cocktorrent {
/**
 * @class TorrentSingleFileInfo
 * @brief Класс для хранения инфы в случае одного файлов
 * @author https://github.com/s3ponia
 */
class TorrentSingleFileInfo : public TorrentBaseFileInfo {
 public:
  using Dictionary = TorrentBaseFileInfo::Dictionary;
  using String = TorrentBaseFileInfo::String;
  using Integer = TorrentBaseFileInfo::Integer;
  using BencodeElement = TorrentBaseFileInfo::BencodeElement;
  using BencodeAdapter = TorrentBaseFileInfo::BencodeAdapter;

  TorrentSingleFileInfo() = delete;

  explicit TorrentSingleFileInfo(BencodeElement const &el)
      : TorrentBaseFileInfo(el) {
    TorrentBaseFileInfo::data_size_ = adapt(&el)["info"]["length"].integer();
  }

  [[nodiscard]] Integer length() const {
    return TorrentBaseFileInfo::data_size_;
  }
};
}  // namespace cocktorrent

#endif  // COCKTORRENT_TORRENTSINGLEFILEINFO_H
