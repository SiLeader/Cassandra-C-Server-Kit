//
// Created by cerussite on 2/16/20.
//

#pragma once

#include <boost/optional.hpp>
#include <cpp17/span.hpp>

#include "common.hpp"

namespace cqlsvrkit {

class Value {
 public:
  enum Type {
    kValue,
    kNull,
    kNotSet,
    kInvalid,
  };

 private:
  Type type_;
  boost::optional<cpp17::span<byte>> value_;

 public:
  explicit Value(Type type) : type_(type) {}
  explicit Value(cpp17::span<byte> value)
      : type_(Type::kValue), value_(value) {}

  Value() = default;
  Value(const Value&) = default;
  Value(Value&&) = default;

  Value& operator=(const Value&) = default;
  Value& operator=(Value&&) = default;

 public:
  static Value Make(const std::vector<byte>& body,
                    std::size_t* offset = nullptr);

 public:
  Type type() const noexcept { return type_; }
};

}  // namespace cqlsvrkit
