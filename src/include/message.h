#ifndef COCKTORRENT_MESSAGE_H
#define COCKTORRENT_MESSAGE_H

#include <boost/asio.hpp>

namespace cocktorrent::peer::tcp {
class PeerMessage {
 public:
  enum class Type {
    KEEP_ALIVE,
    CHOKE,
    UNCHOKE,
    INTERESTED,
    NOT_INTERESTED,
    HAVE,
    BITFIELD,
    REQUEST
  };

  using Buffer = boost::asio::streambuf;

  explicit PeerMessage(const Buffer &);

  Type type() const { return type_; }

  const Buffer &buffer() const { return payload_; }

 private:
  Type type_;
  Buffer payload_;
};
}  // namespace cocktorrent::peer::tcp

#endif  // COCKTORRENT_MESSAGE_H