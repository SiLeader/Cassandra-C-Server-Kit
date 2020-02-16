//
// Created by cerussite on 2/16/20.
//

#pragma once

#include <cstdint>
#include <vector>

#include "common.hpp"
#include "result/rows.hpp"

namespace cqlsvrkit {

struct Void {
  std::vector<byte> toBytes() const { return {}; }
};
class Keyspace {};
class Prepared {};
class SchemaChanged {};

// RESULT
class Result {
 public:
  enum class Kind : std::int32_t {
    kVoid = 0x0001,
    kRows = 0x0002,
    kSetKeyspace = 0x0003,
    kPrepared = 0x0004,
    kSchemaChange = 0x0005,
  };

 private:
  using InternalPayloadType =
      boost::variant<Void, Rows, Keyspace, Prepared, SchemaChanged>;

 private:
  Kind kind_;
  InternalPayloadType payload_;

 public:
  template <class PayloadAssignable>
  Result(Kind kind, PayloadAssignable payload)
      : kind_(kind), payload_(std::move(payload)) {}

 public:
  std::vector<byte> toBytes() const;

 public:
  static Result Make(Void v) { return {Kind::kVoid, std::move(v)}; }
  static Result Make(Rows);
  static Result Make(Keyspace);
  static Result Make(Prepared);
  static Result Make(SchemaChanged);
};

}  // namespace cqlsvrkit
