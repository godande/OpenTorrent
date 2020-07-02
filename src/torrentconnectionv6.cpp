//
// Created by geodesia on 09.06.2020.
//

#include "torrentconnectionv6.h"
#include <random>
#include <string>
#include "logger.h"
#include "utilities.h"

void TorrentConnectionv6::HandShake() {}

uint32_t TorrentConnectionv6::SendConnectRequest() {
  using namespace std::string_literals;
  Buffer buffer;
  std::ostream os(&buffer);
  // magick number
  os.write(util::ToNetworkCharSequence((uint64_t)0x41727101980ll).chars,
           sizeof(uint64_t));
  // action
  os.write(util::ToNetworkCharSequence(uint32_t{}).chars, sizeof(uint32_t));
  std::uniform_int_distribution<uint32_t> distribution;
  uint32_t transaction_id = distribution(generator);
  // transaction id
  os.write(util::ToNetworkCharSequence(transaction_id).chars, sizeof(uint32_t));

  socket_.async_send(
      buffer.data(),
      [ep = end_point_](const ErrorCode &error, std::size_t bytes_transferred) {
        if (error) {
          Logger::get_instance()->Error(error.message() + " at endpoint "s +
                                        ep.address().to_string());
        }
        Logger::get_instance()->Info(std::to_string(bytes_transferred) +
                                     " bytes transferred to "s +
                                     ep.address().to_string());
      });

  return transaction_id;
}

void TorrentConnectionv6::ReceiveConnectResponse(uint32_t transaction_id) {}

void TorrentConnectionv6::SendAnnounceRequest() {
  using namespace std::string_literals;
  Buffer buffer;
  std::ostream os(&buffer);
}
std::mt19937 TorrentConnectionv6::generator{std::random_device{}()};
