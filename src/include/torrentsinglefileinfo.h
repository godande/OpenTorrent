//
// Created by Linux Oid on 24.04.2020.
//

#ifndef COCKTORRENT_TORRENTSINGLEFILEINFO_H
#define COCKTORRENT_TORRENTSINGLEFILEINFO_H

#include "bencode.h"
#include "bencodeelementadapter.h"
#include "torrentbasefileinfo.h"

namespace opentorrent {
class TorrentSingleFileInfo : public TorrentBaseFileInfo {
 public:
  using Dictionary = TorrentBaseFileInfo::Dictionary;
  using String = TorrentBaseFileInfo::String;
  using Integer = TorrentBaseFileInfo::Integer;
  using BencodeElement = TorrentBaseFileInfo::BencodeElement;
  using BencodeAdapter = TorrentBaseFileInfo::BencodeAdapter;

  TorrentSingleFileInfo() = delete;

  explicit TorrentSingleFileInfo(BencodeElement const &el);

  Integer length() const;

 private:
  Integer length_{};
};
}  // namespace opentorrent

#endif  // COCKTORRENT_TORRENTSINGLEFILEINFO_H
