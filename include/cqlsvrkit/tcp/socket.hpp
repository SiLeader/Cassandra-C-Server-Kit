//
// Created by cerussite on 2/11/20.
//

#pragma once

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>

#include "detail/byte.hpp"
#include "detail/data_convert.hpp"

namespace cqlsvrkit {
namespace tcp {

class Socket {
 public:
  using Bytes = std::vector<byte>;
  template <class DataType>
  using OnReadDataCallback = std::function<void(boost::optional<DataType>)>;
  using OnReadBytesCallback = OnReadDataCallback<Bytes>;

  using OnWriteCallback = std::function<void(bool)>;

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

 private:  // read
  template <class DataType>
  void onRead(OnReadDataCallback<DataType> callback,
              const boost::system::error_code& ec,
              std::size_t bytes_transferred) {
    boost::optional<DataType> data;
    if (ec) {
      std::cerr << "read error: " << ec.message() << std::endl;
    } else {
      const byte* bytes = boost::asio::buffer_cast<const byte*>(buffer_.data());
      data = detail::CreateDataFromBytes<DataType>(bytes, bytes_transferred);
      buffer_.consume(bytes_transferred);
    }

    callback(data);
  }

 private:
  template <class DataType>
  void readImpl(std::size_t length, OnReadDataCallback<DataType> callback) {
    boost::asio::async_read(
        *socket_, buffer_, boost::asio::transfer_exactly(length),
        boost::bind(&Socket::onRead<DataType>, this, callback,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

 public:
  template <class DataType>
  void read(OnReadDataCallback<DataType> callback) {
    static_assert(std::is_trivially_copyable<DataType>::value,
                  "read data type must be trivially copyable");
    readImpl<DataType>(sizeof(DataType), std::move(callback));
  }

  void read(std::size_t length, OnReadBytesCallback callback) {
    readImpl<Bytes>(length, std::move(callback));
  }

 private:  // write
  void onWrite(OnWriteCallback callback, const boost::system::error_code& ec,
               std::size_t) {
    if (ec) {
      std::cerr << "read error: " << ec.message() << std::endl;
    }
    callback(!ec);
  }

 public:
  template <class DataType>
  void write(const DataType& data, OnWriteCallback callback) {
    boost::asio::async_write(
        *socket_, boost::asio::buffer(data),
        boost::bind(&Socket::onWrite, this, std::move(callback),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

 public:
  void close() { socket_->close(); }

  std::uint16_t localPort() const { return socket_->local_endpoint().port(); }
  std::uint16_t remotePort() const { return socket_->remote_endpoint().port(); }
};

}  // namespace tcp
}  // namespace cqlsvrkit
