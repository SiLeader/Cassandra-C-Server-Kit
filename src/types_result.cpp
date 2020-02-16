//
// Created by cerussite on 2/16/20.
//

#include <cqlsvrkit/detail/convert.hpp>
#include <cqlsvrkit/types/result.hpp>

namespace cqlsvrkit {

std::vector<byte> Result::toBytes() const {
  auto bytes = convert::toBytes(static_cast<std::int32_t>(kind_));
  bytes.insert(std::end(bytes), std::begin(payload_), std::end(payload_));
  return bytes;
}

}  // namespace cqlsvrkit
