//
// Created by Linux Oid on 28.04.2020.
//

#ifndef COCKTORRENT_TORRENTMULTIPLEFILEINFO_H
#define COCKTORRENT_TORRENTMULTIPLEFILEINFO_H

#include <vector>
#include "bencode.h"
#include "bencodeelementadapter.h"
#include "torrentbasefileinfo.h"

namespace cocktorrent {
/**
 * @class TorrentMultipleFileInfo
 * @brief Класс для хранения инфы в случае нескольких файлов
 * @author https://github.com/s3ponia
 */
class TorrentMultipleFileInfo : public TorrentBaseFileInfo {
 public:
  struct File;
  using InfoDictionary = TorrentBaseFileInfo::Dictionary;
  using String = TorrentBaseFileInfo::String;
  using Integer = TorrentBaseFileInfo::Integer;
  using List = std::vector<File>;
  using BencodeElement = TorrentBaseFileInfo::BencodeElement;
  using BencodeAdapter = TorrentBaseFileInfo::BencodeAdapter;

  struct File {
    using PathList = std::vector<String>;

    explicit File(BencodeElement const &dict)
        : length(adapt(&dict)["length"].integer()),
          path(PathListFromBencode(adapt(&dict)["path"].list())) {}

    static PathList PathListFromBencode(const BencodeAdapter::ListType &el) {
      TorrentMultipleFileInfo::File::PathList path;
      path.reserve(el.size());
      std::for_each(std::cbegin(el), std::cend(el), [&](auto &&el) {
        path.push_back(adapt(&el).string());
      });
      return path;
    }

    Integer length;
    PathList path;
  };

  explicit TorrentMultipleFileInfo(BencodeElement const &el)
      : TorrentBaseFileInfo(el),
        files_{adapt(&el)["info"]["files"].list().cbegin(),
               adapt(&el)["info"]["files"].list().cend()} {
    std::for_each(std::cbegin(files_), std::cend(files_), [&](auto &&el) {
      TorrentBaseFileInfo::data_size_ += el.length;
    });
  }

  [[nodiscard]] const List &files() const { return files_; }

 private:
  List files_;
};
}  // namespace cocktorrent

#endif  // COCKTORRENT_TORRENTMULTIPLEFILEINFO_H
