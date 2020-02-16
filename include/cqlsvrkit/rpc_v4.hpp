//
// Created by cerussite on 2/15/20.
//

#pragma once

#include "rpc_base.hpp"

namespace cqlsvrkit {

class RpcV4 : public cqlsvrkit::RpcBase {
 public:
  using RpcBase::RpcBase;

 public:
  std::unique_ptr<StartupResponse> startup(
      std::uint16_t client_id, boost::string_view cql_version,
      boost::string_view compression) override;
  Supported options(std::uint16_t client_id) override;
  Result query(std::uint16_t client_id, boost::string_view query,
               const QueryParameter& parameter) override;
  Result prepare(std::uint16_t client_id, boost::string_view query) override;
  Result execute(std::uint16_t client_id, cpp17::span<byte> id,
                 const QueryParameter& parameter) override;

  std::unique_ptr<Ready> reg(std::uint16_t client_id,
                             const std::vector<boost::string_view>&) override;
};

}  // namespace cqlsvrkit
