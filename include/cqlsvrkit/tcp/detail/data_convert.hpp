//
// Created by cerussite on 2/12/20.
//

#pragma once

#include <vector>

#include "byte.hpp"

namespace cqlsvrkit {
namespace tcp {
namespace detail {

template <class DataType>
DataType CreateDataFromBytes(const byte* bytes, std::size_t) {
  return *reinterpret_cast<const DataType*>(bytes);
}

template <>
inline std::vector<byte> CreateDataFromBytes<std::vector<byte>>(
    const byte* bytes, std::size_t length) {
  return {bytes, bytes + length};
}

}  // namespace detail
}  // namespace tcp
}  // namespace cqlsvrkit
