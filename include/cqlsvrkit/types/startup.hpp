//
// Created by cerussite on 2/16/20.
//

#pragma once

#include <memory>
#include <vector>

#include "common.hpp"
#include "header.hpp"

namespace cqlsvrkit {

// startup responses
class StartupResponse {
 public:
  virtual ~StartupResponse() = default;
  virtual std::vector<byte> toBytes() const = 0;
  virtual types::OpCode opcode() const = 0;
};

class Ready : public StartupResponse {
 public:
  types::OpCode opcode() const override { return types::OpCode::kReady; }

  std::vector<byte> toBytes() const override { return {}; }

 public:
  static std::unique_ptr<Ready> Make() { return std::make_unique<Ready>(); }
};

class Authenticate : public StartupResponse {
 public:
  types::OpCode opcode() const override { return types::OpCode::kAuthenticate; }
};

}  // namespace cqlsvrkit
