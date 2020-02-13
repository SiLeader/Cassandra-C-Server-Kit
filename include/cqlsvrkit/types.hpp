//
// Created by cerussite on 2/13/20.
//

#pragma once

#include <vector>

#include "tcp/detail/byte.hpp"
#include "types/common.hpp"

namespace cqlsvrkit {

using tcp::byte;

namespace types {

enum class Version : common_types::Byte {
  kRequestV3 = 0x03,
  kResponseV3 = 0x83,
  kRequestV4 = 0x04,
  kResponseV4 = 0x84,
  kRequestV5 = 0x05,
  kResponseV5 = 0x85,
};

enum class OpCode : common_types::Byte {
  kError = 0x00,
  kStartup = 0x01,
  kReady = 0x02,
  kAuthenticate = 0x03,
  kOptions = 0x05,
  kSupported = 0x06,
  kQuery = 0x07,
  kResult = 0x08,
  kPrepare = 0x09,
  kExecute = 0x0a,
  kRegister = 0x0b,
  kEvent = 0x0c,
  kBatch = 0x0d,
  kAuthChallenge = 0x0e,
  kAuthResponse = 0x0f,
  kAuthSuccess = 0x10,
};

namespace values {

enum Flags : common_types::Byte {
  kCompression = 0x01,
  kTracing = 0x02,
  kCustomPayload = 0x04,
  kWarning = 0x08,
  kUseBeta = 0x10,
};

}  // namespace values

struct Header {
  Version version;
  common_types::Byte flags;
  common_types::Short stream;
  OpCode opcode;
  common_types::Int length;
};

}  // namespace types

enum class Consistency : common_types::Short {
  kAny = 0x0000,
  kOne = 0x0001,
  kTwo = 0x0002,
  kThree = 0x0003,
  kQuorum = 0x0004,
  kAll = 0x0005,
  kLocalQuorum = 0x0006,
  kEachQuorum = 0x0007,
  kSerial = 0x0008,
  kLocalSerial = 0x0009,
  kLocalOne = 0x000a,
};

namespace query_flags {

enum Flags {
  kValues = 0x01,
  kSkipMetadata = 0x02,
  kPageSize = 0x04,
  kWithPagingState = 0x08,
  kWithSerialConsistency = 0x10,
  kWithDefaultTimestamp = 0x20,
  kWIthNamesForValues = 0x40,
};

}  // namespace query_flags

class Supported {
 private:
  std::vector<std::string> versions_;
  std::vector<std::string> compression_;

 public:
  Supported(std::vector<std::string> versions,
            std::vector<std::string> compression);

  Supported(const Supported&) = default;
  Supported(Supported&&) = default;

  Supported& operator=(const Supported&) = default;
  Supported& operator=(Supported&&) = default;
  ~Supported() = default;

 public:
  std::vector<byte> toBytes() const;
};

class QueryParameter {
 private:
  Consistency consistency_;
  query_flags::Flags flags_;
};

// startup responses
class StartupResponse {
 public:
  virtual std::vector<byte> toBytes() const = 0;
};

class Ready : public StartupResponse {};
class Authenticate : public StartupResponse {};

// RESULT
class Result {
 public:
  std::vector<byte> toBytes() const;
};

}  // namespace cqlsvrkit
