//
// Created by cerussite on 2/13/20.
//

#pragma once

#include <cstdint>
#include <vector>

#include <boost/optional.hpp>
#include <boost/utility/string_view.hpp>
#include <cpp17/span.hpp>

#include "../tcp/detail/byte.hpp"

namespace cqlsvrkit {

using tcp::byte;

struct Uuid {
  std::uint64_t data1, data2;

  bool operator==(const Uuid& rhs) const noexcept {
    return data1 == rhs.data1 && data2 == rhs.data2;
  }
  bool operator!=(const Uuid& rhs) const noexcept { return !((*this) == rhs); }
};

}  // namespace cqlsvrkit