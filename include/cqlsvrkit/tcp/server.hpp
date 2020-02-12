//
// Created by cerussite on 2/11/20.
//

#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>
#include <iostream>

#include "socket.hpp"

namespace cqlsvrkit {
namespace tcp {

class TcpServer {
 private:
  boost::asio::io_context ioc_;
  boost::asio::ip::tcp::acceptor acceptor_;

  std::function<void(std::shared_ptr<Socket>)> on_accept_callback_;

 public:
  explicit TcpServer(std::uint16_t port)
      : ioc_(),
        acceptor_(ioc_, boost::asio::ip::tcp::endpoint(
                            boost::asio::ip::tcp::v4(), port)) {}

  TcpServer(const TcpServer&) = delete;
  TcpServer(TcpServer&&) = delete;

  TcpServer& operator=(const TcpServer&) = delete;
  TcpServer& operator=(TcpServer&&) = delete;

  ~TcpServer() = default;

 private:
  void onAccept(std::unique_ptr<boost::asio::ip::tcp::socket> socket,
                const boost::system::error_code& ec) {
    if (ec) {
      std::cerr << "accept error: " << ec.message() << std::endl;
    } else {
      auto sock = Socket::Make(std::move(socket));
      ioc_.post([this, sock] { on_accept_callback_(sock); });
    }
    listenImpl();
  }

 private:
  void listenImpl() {
    auto socket = std::make_unique<boost::asio::ip::tcp::socket>(ioc_);
    auto socket_ptr = socket.get();
    acceptor_.async_accept(
        *socket_ptr, boost::bind(&TcpServer::onAccept, this, std::move(socket),
                                 boost::asio::placeholders::error));
  }

 public:
  template <class CallbackF>
  void listen(CallbackF&& callback) {
    on_accept_callback_ = std::forward<CallbackF>(callback);
    listenImpl();
  }
};

}  // namespace tcp
}  // namespace cqlsvrkit
