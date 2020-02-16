//
// Created by cerussite on 2/15/20.
//

#include <cqlsvrkit/detail/convert.hpp>

namespace cqlsvrkit {
namespace convert {

std::vector<byte> toBytes(const std::string& data) {
  auto bytes = toBytes(static_cast<std::int16_t>(data.size()));
  auto ptr = reinterpret_cast<const byte*>(data.data());
  bytes.insert(std::end(bytes), ptr, ptr + data.size());

  return bytes;
}

std::vector<byte> toBytes(const boost::string_view& data) {
  auto bytes = toBytes(static_cast<std::int16_t>(data.size()));
  auto ptr = reinterpret_cast<const byte*>(data.data());
  bytes.insert(std::end(bytes), ptr, ptr + data.size());

  return bytes;
}

boost::string_view toString(const std::vector<byte>& data,
                            std::size_t* offset) {
  return toSequentialDataImpl<boost::string_view, std::int16_t>(data, offset);
}

boost::string_view toLongString(const std::vector<byte>& data,
                                std::size_t* offset) {
  return toSequentialDataImpl<boost::string_view, std::int32_t>(data, offset);
}

std::map<boost::string_view, boost::string_view> toStringMap(
    const std::vector<byte>& data, std::size_t* offset) {
  auto offset_value = offset == nullptr ? 0 : *offset;
  detail::convert_assert(data.size() >= offset_value,
                         "invalid request body length");

  auto length =
      static_cast<std::size_t>(toInteger<std::int16_t>(data, &offset_value));
  std::map<boost::string_view, boost::string_view> buffer;
  for (std::size_t i = 0; i < length; ++i) {
    auto key = toString(data, &offset_value);
    auto value = toString(data, &offset_value);
    buffer[key] = value;
  }
  if (offset) {
    *offset = offset_value;
  }

  return buffer;
}

cpp17::span<byte> toShortBytes(const std::vector<byte>& data,
                               std::size_t* offset) {
  return toSequentialDataImpl<cpp17::span<byte>, std::uint16_t>(data, offset);
}

cpp17::span<byte> toBytes(const std::vector<byte>& data, std::size_t* offset) {
  return toSequentialDataImpl<cpp17::span<byte>, std::uint32_t>(data, offset);
}

std::vector<boost::string_view> toStringList(const std::vector<byte>& data,
                                             std::size_t* offset) {
  auto offset_value = offset == nullptr ? 0 : *offset;
  detail::convert_assert(data.size() >= offset_value,
                         "invalid request body length");

  auto length = toInteger<std::uint16_t>(data, &offset_value);
  std::vector<boost::string_view> sl;
  sl.reserve(length);

  for (std::size_t i = 0; i < length; ++i) {
    sl.emplace_back(toString(data, &offset_value));
  }
  if (offset) {
    *offset = offset_value;
  }

  return sl;
}

cpp17::span<byte> toValue(const std::vector<byte>& data, std::size_t* offset) {
  return toSequentialDataImpl<cpp17::span<byte>, std::uint32_t>(data, offset);
}

}  // namespace convert
}  // namespace cqlsvrkit
