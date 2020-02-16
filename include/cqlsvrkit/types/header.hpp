//
// Created by cerussite on 2/16/20.
//

#pragma once

#include "common.hpp"

namespace cqlsvrkit {

enum class Consistency : std::int16_t {
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

namespace types {

enum class Version : std::uint8_t {
  kRequestV3 = 0x03,
  kResponseV3 = 0x83,
  kRequestV4 = 0x04,
  kResponseV4 = 0x84,
  kRequestV5 = 0x05,
  kResponseV5 = 0x85,
};

enum class OpCode : std::uint8_t {
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

enum Flags : std::uint8_t {
  kCompression = 0x01,
  kTracing = 0x02,
  kCustomPayload = 0x04,
  kWarning = 0x08,
  kUseBeta = 0x10,
};

}  // namespace values

struct [[gnu::packed]] Header {
  Version version;
  std::uint8_t flags;
  std::int16_t stream;
  OpCode opcode;
  std::int32_t length;
};

}  // namespace types
}  // namespace cqlsvrkit
