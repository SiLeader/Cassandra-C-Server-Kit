//
// Created by cerussite on 2/13/20.
//

#pragma once

#include <cstdint>

#include <boost/utility/string_view.hpp>
#include <cpp17/span.hpp>

namespace cqlsvrkit {

namespace common_types {

using Int = std::int32_t;
using Long = std::int64_t;
using Byte = std::uint8_t;
using Short = std::uint16_t;

struct Uuid {
  std::uint64_t data1, data2;

  bool operator==(const Uuid& rhs) const noexcept {
    return data1 == rhs.data1 && data2 == rhs.data2;
  }
  bool operator!=(const Uuid& rhs) const noexcept { return !((*this) == rhs); }
};

template <class T>
using List = cpp17::span<T>;
using String = boost::string_view;
using LongString = String;
using StringList = List<String>;
using Bytes = List<Byte>;
using ShortBytes = Bytes;

}  // namespace common_types
}  // namespace cqlsvrkit