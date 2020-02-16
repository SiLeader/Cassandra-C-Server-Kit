//
// Created by cerussite on 2/13/20.
//

#include <cqlsvrkit/detail/convert.hpp>
#include <cqlsvrkit/rpc_base.hpp>

namespace cqlsvrkit {

void RpcBase::onAccept(std::shared_ptr<tcp::Socket> socket) {
  socket->read<types::Header>(
      std::bind(&RpcBase::onHeaderRead, this, socket, std::placeholders::_1));
}

void RpcBase::onHeaderRead(std::shared_ptr<tcp::Socket> socket,
                           boost::optional<types::Header> opt_header) {
  if (!opt_header) {
    socket->close();
    return;
  }

  auto header = opt_header.value();
  header.length = ntohl(header.length);
  // header.stream = ntohs(header.stream);

  if (header.length == 0) {
    ioc_.post([this, sock = std::move(socket), header] {
      finishRequest(sock, header, boost::none);
    });
  } else {
    socket->read(header.length, std::bind(&RpcBase::finishRequest, this, socket,
                                          header, std::placeholders::_1));
  }
}

void RpcBase::finishRequest(std::shared_ptr<tcp::Socket> socket,
                            types::Header header,
                            boost::optional<std::vector<tcp::byte>> opt_body) {
  static const auto create_error = [](error::ErrorCode code,
                                      const char* message,
                                      types::Header& rh) -> std::vector<byte> {
    auto bytes = convert::toBytes(static_cast<std::int32_t>(code));
    auto message_bytes = convert::toBytes(boost::string_view{message});
    bytes.insert(std::end(bytes), std::begin(message_bytes),
                 std::end(message_bytes));
    rh.opcode = types::OpCode::kError;
    return bytes;
  };

  types::Header response_header = header;
  response_header.version = types::Version::kResponseV4;

  auto id = socket->remotePort();
  std::vector<byte> body_bytes;

  try {
    if (header.opcode != types::OpCode::kOptions && !opt_body.has_value()) {
      throw error::ProtocolError("no body was provided");
    }

    switch (header.opcode) {
      case types::OpCode::kStartup:
        body_bytes = startupCaller(id, response_header, opt_body.value());
        break;
      case types::OpCode::kOptions:
        body_bytes = optionsCaller(id, response_header);
        break;
      case types::OpCode::kQuery:
        body_bytes = queryCaller(id, response_header, opt_body.value());
        break;
      case types::OpCode::kPrepare:
        body_bytes = prepareCaller(id, response_header, opt_body.value());
        break;
      case types::OpCode::kExecute:
        body_bytes = executeCaller(id, response_header, opt_body.value());
        break;
      case types::OpCode::kRegister:
        body_bytes = registerCaller(id, response_header, opt_body.value());
        break;

        // not supported
      case types::OpCode::kAuthResponse:
      case types::OpCode::kBatch:
        throw error::ServerError("not implemented");
      default:  // not request for server
        throw error::ProtocolError("unknown request type");
    }
  } catch (const error::CqlException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    body_bytes = create_error(e.code(), e.what(), response_header);
  } catch (...) {
    body_bytes = create_error(error::ErrorCode::kServerError,
                              "exception thrown", response_header);
  }

  response_header.length = htonl(body_bytes.size());
  auto response_header_bp = reinterpret_cast<const byte*>(&response_header);
  body_bytes.insert(std::begin(body_bytes), response_header_bp,
                    response_header_bp + sizeof(response_header));

  socket->write(body_bytes, std::bind(&RpcBase::onResponseWrote, this, socket,
                                      std::placeholders::_1));
}

void RpcBase::onResponseWrote(std::shared_ptr<tcp::Socket> socket, bool ok) {
  if (!ok) {
    socket->close();
    return;
  }

  onAccept(std::move(socket));
}

// callers
std::vector<byte> RpcBase::startupCaller(std::uint16_t client_id,
                                         types::Header& response_header,
                                         const std::vector<tcp::byte>& body) {
  auto sm = convert::toStringMap(body);
  auto cql_version = sm["CQL_VERSION"];
  auto compression = sm["COMPRESSION"];
  auto res = startup(client_id, cql_version, compression);

  response_header.opcode = res->opcode();

  return convert::toBytes(res);
}

std::vector<byte> RpcBase::optionsCaller(std::uint16_t client_id,
                                         types::Header& response_header) {
  response_header.opcode = types::OpCode::kSupported;

  return convert::toBytes(options(client_id));
}

std::vector<byte> RpcBase::queryCaller(std::uint16_t client_id,
                                       types::Header& response_header,
                                       const std::vector<tcp::byte>& body) {
  std::size_t offset = 0;
  auto query_str = convert::toLongString(body, &offset);
  auto parameter = QueryParameter::Make(body, &offset);

  response_header.opcode = types::OpCode::kResult;

  return convert::toBytes(query(client_id, query_str, parameter));
}

std::vector<byte> RpcBase::prepareCaller(std::uint16_t client_id,
                                         types::Header& response_header,
                                         const std::vector<tcp::byte>& body) {
  auto query_str = convert::toLongString(body);

  response_header.opcode = types::OpCode::kResult;

  return convert::toBytes(prepare(client_id, query_str));
}

std::vector<byte> RpcBase::executeCaller(std::uint16_t client_id,
                                         types::Header& response_header,
                                         const std::vector<tcp::byte>& body) {
  std::size_t offset = 0;
  auto id = convert::toShortBytes(body, &offset);
  auto parameter = QueryParameter::Make(body, &offset);

  response_header.opcode = types::OpCode::kResult;

  return convert::toBytes(execute(client_id, id, parameter));
}

std::vector<byte> RpcBase::registerCaller(std::uint16_t client_id,
                                          types::Header& response_header,
                                          const std::vector<tcp::byte>& body) {
  std::cout << body.size() << std::endl;
  auto register_list = convert::toStringList(body);
  auto response = reg(client_id, register_list);

  response_header.opcode = response->opcode();
  std::cout << "register processed" << std::endl;

  return convert::toBytes(response);
}

RpcBase::RpcBase(std::uint16_t port) : ioc_(), server_(ioc_, port) {
  server_.listen(std::bind(&RpcBase::onAccept, this, std::placeholders::_1));
}

RpcBase::~RpcBase() {
  for (auto& thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void RpcBase::startAndWait(std::size_t threads) {
  start(threads - 1);
  server_.run();
}

void RpcBase::start(std::size_t threads) {
  for (std::size_t i = 0; i < threads; ++i) {
    threads_.emplace_back([this] { server_.run(); });
  }
}

}  // namespace cqlsvrkit
