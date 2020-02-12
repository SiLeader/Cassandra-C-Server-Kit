//
// Created by cerussite on 2/11/20.
//

#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace cqlsvrkit {
namespace tcp {

enum class byte : std::uint8_t {};

class Socket {
 public:
  using Bytes = std::vector<byte>;
  template <class DataType>
  using OnReadDataCallback = std::function<void(const DataType&)>;
  using OnReadBytesCallback = OnReadDataCallback<Bytes>;

 private:
  std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
  boost::asio::streambuf buffer_;

 public:
  explicit Socket(std::unique_ptr<boost::asio::ip::tcp::socket> socket)
      : socket_(std::move(socket)) {}

  static std::shared_ptr<Socket> Make(
      std::unique_ptr<boost::asio::ip::tcp::socket> socket) {
    return std::make_shared<Socket>(std::move(socket));
  }

  Socket(const Socket&) = delete;
  Socket(Socket&&) = delete;

  Socket& operator=(const Socket&) = delete;
  Socket& operator=(Socket&&) = delete;

  ~Socket() = default;

 public:
  std::unique_ptr<boost::asio::ip::tcp::socket>& socket() noexcept {
    return socket_;
  }

  const std::unique_ptr<boost::asio::ip::tcp::socket>& socket() const noexcept {
    return socket_;
  }

 private:
  template <class DataType>
  void onRead(OnReadDataCallback<DataType> callback,
              const boost::system::error_code& error,
              size_t bytes_transferred) {}

 private:
  template <class DataType>
  void readImpl(std::size_t length, OnReadDataCallback<DataType> callback) {
    boost::asio::async_read_until(
        *socket_, buffer_, boost::asio::transfer_exactly(length),
        boost::bind(&Socket::onRead<DataType>, this, callback,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

 public:
  template <class DataType>
  void read(OnReadDataCallback<DataType> callback) {
    readImpl<DataType>(sizeof(DataType), std::move(callback));
  }

  void read(std::size_t length, OnReadBytesCallback callback) {
    readImpl<Bytes>(length, std::move(callback));
  }
};

}  // namespace tcp
}  // namespace cqlsvrkit
