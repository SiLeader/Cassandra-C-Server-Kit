//
// Created by cerussite on 2/16/20.
//

#include <cqlsvrkit/detail/convert.hpp>
#include <cqlsvrkit/types/value.hpp>

namespace cqlsvrkit {

Value Value::Make(const std::vector<tcp::byte>& body, std::size_t* offset) {
  std::size_t offset_value = 0;
  if (!offset) {
    offset = &offset_value;
  }
  convert::detail::convert_assert(body.size() >= *offset,
                                  "invalid request body length");

  auto length = convert::toInteger<std::int32_t>(body, offset);
  if (length == -1) {
    return Value{Value::Type::kNull};
  } else if (length == -2) {
    return Value{Value::Type::kNotSet};
  } else if (length < -2) {
    return Value{Value::Type::kInvalid};
  }

  cpp17::span<byte> bytes(body.data() + (*offset), length);

  return Value{bytes};
}

}  // namespace cqlsvrkit
