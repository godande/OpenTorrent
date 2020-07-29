//
// Created by Linux Oid on 28.04.2020.
//

#ifndef COCKTORRENT_TORRENTBASEFILEINFO_H
#define COCKTORRENT_TORRENTBASEFILEINFO_H

#include <type_traits>
#include <vector>
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
  using AnnounceList = std::vector<std::string>;
  using InfoHashType = std::array<char, 20>;

  enum class DownloadState {
    NONE = 0,
    COMPLETED = 1,
    STARTED = 2,
    STOPPED = 3
  };

  enum class LocalState { Leecher = 0, Seeder = 1 };

  [[nodiscard]] static Integer ToInteger(const DownloadState &d_state) {
    return static_cast<std::underlying_type_t<DownloadState>>(d_state);
  };

  TorrentBaseFileInfo() = delete;

  explicit TorrentBaseFileInfo(BencodeElement const &el);

  [[nodiscard]] LocalState local_state() const {
    auto l_state = state();
    switch (l_state) {
      case DownloadState::NONE:
      case DownloadState::STARTED:
        return LocalState::Leecher;
      default:
        return LocalState::Seeder;
    }
  }

  [[nodiscard]] const String &announce() const { return announce_; }

  [[nodiscard]] const AnnounceList &announce_list() const {
    return announce_list_;
  }

  [[nodiscard]] const String &pieces() const { return pieces_; }

  [[nodiscard]] const String &name() const { return name_; }

  [[nodiscard]] const InfoHashType &info_hash() const { return info_hash_; }

  [[nodiscard]] Integer piece_length() const { return piece_length_; }

  [[nodiscard]] Integer uploaded() const { return 0; }

  [[nodiscard]] Integer downloaded() const { return 0; }

  [[nodiscard]] Integer left() const { return data_size_; }

  [[nodiscard]] DownloadState state() const { return DownloadState::NONE; }

 protected:
  Integer data_size_{};

 private:
  String announce_{};
  AnnounceList announce_list_{};
  String pieces_{};
  String name_{};
  InfoHashType info_hash_{};
  Integer piece_length_{};
};  // namespace cocktorrent
}  // namespace cocktorrent

#endif  // COCKTORRENT_TORRENTBASEFILEINFO_H
