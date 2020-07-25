//
// Created by Linux Oid on 24.04.2020.
//

#include "torrentsinglefileinfo.h"

namespace cocktorrent {
TorrentSingleFileInfo::TorrentSingleFileInfo(
    const TorrentSingleFileInfo::BencodeElement &el)
    : TorrentBaseFileInfo(el),
      length_(adapt(&el)["info"]["length"].integer()) {}

TorrentSingleFileInfo::Integer TorrentSingleFileInfo::length() const {
  return length_;
}
}  // namespace cocktorrent
