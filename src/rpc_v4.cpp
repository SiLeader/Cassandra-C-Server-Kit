//
// Created by cerussite on 2/15/20.
//

#include <cqlsvrkit/rpc_v4.hpp>

namespace cqlsvrkit {

std::unique_ptr<StartupResponse> RpcV4::startup(
    std::uint16_t client_id, boost::string_view cql_version,
    boost::string_view compression) {
  std::cout << "startup client: " << client_id << ", cql: " << cql_version
            << ", compression: " << compression << std::endl;
  return Ready::Make();
}

Supported RpcV4::options(std::uint16_t client_id) {
  std::cout << "options client: " << client_id << std::endl;
  return Supported::Make({"3.0.0"}, {});
}

Result RpcV4::query(std::uint16_t client_id, boost::string_view query,
                    const QueryParameter&) {
  std::cout << "query client: " << client_id << ", query" << query << std::endl;
  return Result::Make(Void{});
}

Result RpcV4::prepare(std::uint16_t client_id, boost::string_view query) {
  std::cout << "query client: " << client_id << ", query" << query << std::endl;
  return Result::Make(Void{});
}

Result RpcV4::execute(std::uint16_t client_id, cpp17::span<byte>,
                      const QueryParameter&) {
  std::cout << "query client: " << client_id << std::endl;
  return Result::Make(Void{});
}

std::unique_ptr<Ready> RpcV4::reg(std::uint16_t client_id,
                                  const std::vector<boost::string_view>&) {
  std::cout << "register client: " << client_id << std::endl;
  return Ready::Make();
}

}  // namespace cqlsvrkit
