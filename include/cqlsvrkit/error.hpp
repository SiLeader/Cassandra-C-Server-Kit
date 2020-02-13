//
// Created by cerussite on 2/13/20.
//

#pragma once

#include "types/common.hpp"

namespace cqlsvrkit {
namespace error {

enum class ErrorCode : common_types::Short {
  kServerError = 0x0000,
  kProtocolError = 0x000a,
  kAuthenticationError = 0x0100,
  kUnavailableException = 0x1000,
  kOverloaded = 0x1001,
  kNodeIsBootstrapping = 0x1002,
  kTruncateError = 0x1003,
  kWriteTimeout = 0x1100,
  kReadTimeout = 0x1200,
  kReadFailure = 0x1300,
  kFunctionFailure = 0x1400,
  kWriteFailure = 0x1500,
  kCdcWriteFailure = 0x1600,
  kCasWriteUnknown = 0x1700,
  kSyntaxError = 0x2000,
  kUnauthorized = 0x2100,
  kInvalid = 0x2200,
  kConfigError = 0x2300,
  kAlreadyExists = 0x2400,
  kUnprepared = 0x2500,
};

struct CqlException : std::runtime_error {
 private:
  ErrorCode code_;

 public:
  CqlException(ErrorCode code, const char* message)
      : runtime_error(message), code_(code) {}

 public:
  ErrorCode code() const noexcept { return code_; }
};

#define DEF_EXCEPTION(name)                            \
  struct name : CqlException {                         \
    explicit name(const char* message)                 \
        : CqlException(ErrorCode::k##name, message) {} \
    name() : name(#name) {}                            \
  }

DEF_EXCEPTION(ServerError);
DEF_EXCEPTION(ProtocolError);
DEF_EXCEPTION(AuthenticationError);
DEF_EXCEPTION(Overloaded);
DEF_EXCEPTION(NodeIsBootstrapping);
DEF_EXCEPTION(TruncateError);
DEF_EXCEPTION(WriteTimeout);
DEF_EXCEPTION(ReadTimeout);
DEF_EXCEPTION(ReadFailure);
DEF_EXCEPTION(FunctionFailure);
DEF_EXCEPTION(WriteFailure);
DEF_EXCEPTION(CdcWriteFailure);
DEF_EXCEPTION(CasWriteUnknown);
DEF_EXCEPTION(SyntaxError);
DEF_EXCEPTION(Unauthorized);
DEF_EXCEPTION(Invalid);
DEF_EXCEPTION(ConfigError);
DEF_EXCEPTION(AlreadyExists);
DEF_EXCEPTION(Unprepared);

#undef DEF_EXCEPTION

}  // namespace error
}  // namespace cqlsvrkit
