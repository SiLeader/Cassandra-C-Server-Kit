//
// Created by cerussite on 2/12/20.
//

#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <vector>

#include "error.hpp"
#include "tcp/server.hpp"
#include "tcp/socket.hpp"
#include "types.hpp"

namespace cqlsvrkit {

class RpcBase {
 private:
  boost::asio::io_service ioc_;
  tcp::TcpServer server_;

  std::vector<std::thread> threads_;

 private:  // event handlers
  void onAccept(std::shared_ptr<tcp::Socket> socket);

  void onHeaderRead(std::shared_ptr<tcp::Socket> socket,
                    boost::optional<types::Header> opt_header);

  void finishRequest(std::shared_ptr<tcp::Socket> socket, types::Header header,
                     boost::optional<std::vector<tcp::byte>> opt_body);

  void onResponseWrote(std::shared_ptr<tcp::Socket> socket, bool ok);

 private:  // handler callers
  std::vector<byte> startupCaller(std::uint16_t client_id,
                                  types::Header& response_header,
                                  const std::vector<tcp::byte>& body);

  std::vector<byte> optionsCaller(std::uint16_t client_id,
                                  types::Header& response_header);

  std::vector<byte> queryCaller(std::uint16_t client_id,
                                types::Header& response_header,
                                const std::vector<tcp::byte>& body);

  std::vector<byte> prepareCaller(std::uint16_t client_id,
                                  types::Header& response_header,
                                  const std::vector<tcp::byte>& body);

  std::vector<byte> executeCaller(std::uint16_t client_id,
                                  types::Header& response_header,
                                  const std::vector<tcp::byte>& body);

  std::vector<byte> registerCaller(std::uint16_t client_id,
                                   types::Header& response_header,
                                   const std::vector<tcp::byte>& body);

 public:
  explicit RpcBase(std::uint16_t port);

  virtual ~RpcBase();

 public:
  void startAndWait(std::size_t threads = 1);

  void start(std::size_t threads = 1);

 public:
  virtual std::unique_ptr<StartupResponse> startup(
      std::uint16_t client_id, boost::string_view cql_version,
      boost::string_view compression) = 0;
  // virtual AuthSuccess authResponse(std::uint16_t client_id,
  // cpp17::span<byte>) = 0;
  virtual Supported options(std::uint16_t client_id) = 0;
  virtual Result query(std::uint16_t client_id, boost::string_view query,
                       const QueryParameter& parameter) = 0;
  virtual Result prepare(std::uint16_t client_id, boost::string_view query) = 0;
  virtual Result execute(std::uint16_t client_id, cpp17::span<byte> id,
                         const QueryParameter& parameter) = 0;
  // virtual void batch() = 0; // not supported
  virtual std::unique_ptr<Ready> reg(
      std::uint16_t client_id, const std::vector<boost::string_view>&) = 0;
};

}  // namespace cqlsvrkit
