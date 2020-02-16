//
// Created by cerussite on 2/16/20.
//

#pragma once

#include <cstdint>
#include <vector>

#include "../detail/convert.hpp"
#include "common.hpp"

namespace cqlsvrkit {

class Option {
 private:
  std::int16_t id_;
  std::vector<byte> free_style_data_;

 public:
  Option(std::int16_t id, std::vector<byte> data)
      : id_(id), free_style_data_(std::move(data)) {}

  template <class BytesConvertible>
  Option(std::int16_t id, BytesConvertible data)
      : Option(id, convert::toBytes(std::move(data))) {}
};

}  // namespace cqlsvrkit
