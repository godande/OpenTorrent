#include "include/message.h"
#include <utility>
#include "include/utilities.h"

namespace {

std::pair<PeerMessage::Type, PeerMessage::Buffer> ParseChoke(
    const boost::asio::streambuf &buf) {
  if (!buf.size()) {
    return {PeerMessage::Type::KEEP_ALIVE, PeerMessage::Buffer{}};
  }
}
}  // namespace

PeerMessage::PeerMessage(const boost::asio::streambuf &buf) {}
