//
// Created by Linux Oid on 26.07.2020.
//
#include <udp/trackerconnection.h>
#include <logger.h>
#include <utilities.h>

void cocktorrent::udp::TrackerConnection::ConnHandle(
    const cocktorrent::udp::TrackerConnection::ErrorCode& error_code,
    const cocktorrent::udp::TrackerConnection::EndPoint& endpoint) {
  if (!socket_.is_open()) {
    Logger::get_instance()->Info("Connection request to " +
                                 endpoint.address().to_string() + " expired.");
    TryNext();
  } else if (error_code) {
    Logger::get_instance()->Error("Error in connection to " +
                                  endpoint.address().to_string());
    ErrorCode ec;
    socket_.close(ec);
    if (ec) {
      Logger::get_instance()->Error("Error in closing socket. " + ec.message());
      return;
    }
    TryNext();
  } else {
    Logger::get_instance()->Info("Socket is connected.");
    SendConnection();
  }
}

void cocktorrent::udp::TrackerConnection::ConnIDExpireHandle([
    [maybe_unused]] const cocktorrent::udp::TrackerConnection::ErrorCode&
                                                                 error_code) {
  ErrorCode ec{};
  if (socket_.is_open()) socket_.cancel(ec);
  if (ec || error_code) {
    Logger::get_instance()->Error("Error in resending ConnectionPacket. " +
                                  ec.message());
  } else {
    Logger::get_instance()->Info("Connection ID expired.");
    socket_.cancel();
    connection_id_timer_.expires_at(DeadLineTimer::time_point::max());
    SendConnection();
  }
}

void cocktorrent::udp::TrackerConnection::Connect() {
  if (iterator_ != end_points_.end()) {
    stopped_ = false;
    Logger::get_instance()->Info("Trying " + iterator_->host_name() + ":" +
                                 iterator_->service_name() + "...");
    boost::asio::async_connect(
        socket_, iterator_,
        [this](const ErrorCode& ec, const EndPoints::iterator& endpoint) {
          this->ConnHandle(ec, endpoint->endpoint());
        });
  }
}

void cocktorrent::udp::TrackerConnection::TryNext() {
  Stop();
  time_out_ = TimeOut{15};
  if (iterator_ != end_points_.end()) {
    ++iterator_;
    Logger::get_instance()->Info("Next tracker.");
  }
  Connect();
}

void cocktorrent::udp::TrackerConnection::ConnRequestTimeOut(
    const cocktorrent::udp::TrackerConnection::ErrorCode& ec) {
  if (ec) {
    Logger::get_instance()->Error("Error in Connect request timeout. " +
                                  ec.message());
  } else {
    Logger::get_instance()->Info("Connect request timeout.");
    time_out_ *= 2;
    if (time_out_ >= biggest_timeout_) {
      TryNext();
    } else {
      SendConnection();
    }
  }
}

void cocktorrent::udp::TrackerConnection::ConnPackHandle(
    int32_t transaction_id,
    const cocktorrent::udp::TrackerConnection::ErrorCode& error,
    std::size_t bytes_transferred) {
  if (error || bytes_transferred < 16) {
    Logger::get_instance()->Error("Error in sending ConnectionPacket. " +
                                  std::to_string(bytes_transferred) +
                                  " bytes transferred.");
    SendConnection();
  } else {
    ReceiveConnection(transaction_id);
  }
}

void cocktorrent::udp::TrackerConnection::SendConnection() {
  Logger::get_instance()->Info("Start sending connection request...");
  timer_.cancel();
  timer_.expires_after(time_out_);
  ConnectPacket send_packet{};
  send_conn_buf_ = send_packet.buffer();
  int32_t transaction_id = send_packet.transactionID();
  socket_.async_send(boost::asio::buffer(send_conn_buf_),
                     [this, transaction_id](const ErrorCode& error,
                                            std::size_t bytes_transferred) {
                       this->ConnPackHandle(transaction_id, error,
                                            bytes_transferred);
                     });
  timer_.async_wait(
      [this](const ErrorCode& ec) { this->ConnRequestTimeOut(ec); });
}

void cocktorrent::udp::TrackerConnection::AnnRequestTimeOut(
    int64_t conn_id, const cocktorrent::udp::TrackerConnection::ErrorCode& ec) {
  if (ec) {
    Logger::get_instance()->Error("Error in Announce request timeout. " +
                                  ec.message());
  } else {
    Logger::get_instance()->Info("Announce request timeout.");
    time_out_ *= 2;
    if (time_out_ >= biggest_timeout_) {
      TryNext();
    } else {
      SendAnnounce(conn_id);
    }
  }
}

void cocktorrent::udp::TrackerConnection::ReceiveConnection(
    int32_t transaction_id) {
  socket_.async_receive(
      boost::asio::buffer(receive_conn_buf_),
      [this, transaction_id](const boost::system::error_code& error,
                             std::size_t bytes_transferred) {
        this->ReceiveConnHandle(transaction_id, error, bytes_transferred);
      });
}

void cocktorrent::udp::TrackerConnection::ReceiveConnHandle(
    int32_t transaction_id,
    const cocktorrent::udp::TrackerConnection::ErrorCode& error,
    std::size_t bytes_transferred) {
  if (error || bytes_transferred < 16) {
    Logger::get_instance()->Error("Error in receiving ConnectionPacket. " +
                                  std::to_string(bytes_transferred) +
                                  " bytes transferred.");
  } else {
    Logger::get_instance()->Info("Connection request received.");
    connection_id_timer_.expires_after(TimeOut{60});
    connection_id_timer_.async_wait(
        [this](const ErrorCode& ec) { this->ConnIDExpireHandle(ec); });
    ResponseConnectPacket income{boost::asio::buffer(receive_conn_buf_),
                                 transaction_id};
    SendAnnounce(income.connectionID());
  }
}

void cocktorrent::udp::TrackerConnection::ReceiveAnnHandle(
    int32_t transaction_id, const boost::system::error_code& error,
    std::size_t bytes_transferred) {
  if (error || bytes_transferred < 20) {
    Logger::get_instance()->Error("Error in sending AnnouncePacket. " +
                                  std::to_string(bytes_transferred) +
                                  " bytes transferred.");
    SendAnnounce(transaction_id);
  } else {
    Logger::get_instance()->Info("Announce response received.");
    connection_id_timer_.expires_after(TimeOut{60});
    ResponseAnnouncePacket income{boost::asio::buffer(receive_ann_buf_),
                                  transaction_id};
    if (!income.peers().empty()) {
      auto&& p = income.peers();
      std::copy(std::begin(p), std::end(p), std::back_inserter(peers_));
    }
    TryNext();
  }
}

void cocktorrent::udp::TrackerConnection::ReceiveAnnounce(
    int32_t transaction_id) {
  Logger::get_instance()->Info("Receiving announce response.");
  socket_.async_receive(
      boost::asio::buffer(receive_ann_buf_),
      [this, transaction_id](const boost::system::error_code& error,
                             std::size_t bytes_transferred) {
        this->ReceiveAnnHandle(transaction_id, error, bytes_transferred);
      });
}

void cocktorrent::udp::TrackerConnection::SendAnnHandler(
    int64_t conn_id, int32_t trans_id,
    const cocktorrent::udp::TrackerConnection::ErrorCode& error,
    std::size_t bytes_transferred) {
  if (error) {
    Logger::get_instance()->Error("Error in sending AnnouncePacket. " +
                                  std::to_string(bytes_transferred) +
                                  " bytes transferred.");
    SendAnnounce(conn_id);
  } else {
    Logger::get_instance()->Info("Announce request sent.");
    ReceiveAnnounce(trans_id);
  }
}

void cocktorrent::udp::TrackerConnection::SendAnnounce(int64_t conn_id) {
  Logger::get_instance()->Info("Start sending announce...");
  timer_.cancel();
  timer_.expires_after(time_out_);
  std::array<char, 20> generated{};
  std::uniform_int_distribution<char> distribution;
  std::generate(generated.begin(), generated.end(),
                [&distribution]() { return distribution(util::generator); });
  std::uniform_int_distribution<uint32_t> distribution2;
  AnnouncePacket send_announce(conn_id, info_hash_, generated, 0, 0, 0, 2, 0,
                               distribution2(util::generator), -1, 1337, 0);
  send_ann_buf_ = send_announce.buffer();
  int32_t trans_id = send_announce.transactionID();
  socket_.async_send(boost::asio::buffer(send_ann_buf_),
                     [this, conn_id, trans_id](const ErrorCode& ec,
                                               std::size_t bytes_transferred) {
                       this->SendAnnHandler(conn_id, trans_id, ec,
                                            bytes_transferred);
                     });
  timer_.async_wait([this, conn_id](const ErrorCode& ec) {
    this->AnnRequestTimeOut(conn_id, ec);
  });
}

void cocktorrent::udp::TrackerConnection::Stop() {
  socket_.close();
  timer_.cancel();
  connection_id_timer_.cancel();
  timer_.expires_at(DeadLineTimer::time_point::max());
  connection_id_timer_.expires_at(DeadLineTimer::time_point::max());
  stopped_ = true;
  Logger::get_instance()->Info("Tracker connection stopped.");
}

void cocktorrent::udp::TrackerConnection::Run(
    const cocktorrent::udp::TrackerConnection::EndPoints& end_points) {
  end_points_ = end_points;
  iterator_ = end_points_.begin();
  Connect();
}
