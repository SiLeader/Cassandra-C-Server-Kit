//
// Created by cerussite on 2/13/20.
//

#pragma once

#include <unordered_map>

#include <endian.h>

#include "../types.hpp"

namespace cqlsvrkit {
namespace convert {
std::uint16_t hostToNetwork(std::uint16_t n) { return htobe16(n); }
std::uint32_t hostToNetwork(std::uint32_t n) { return htobe32(n); }
std::uint64_t hostToNetwork(std::uint64_t n) { return htobe64(n); }

template <class Integral,
          class = std::enable_if_t<std::is_integral<Integral>::value>>
std::vector<byte> toBytes(Integral integral) {
  integral = hostToNetwork(integral);
  auto integral_bp = reinterpret_cast<const byte*>(&integral);
  return {integral_bp, integral_bp + sizeof(Integral)};
}

template <class BytesConvertible>
auto toBytes(const BytesConvertible& data) -> decltype(data.toBytes()) {
  return data.toBytes();
}

template <class BytesConvertiblePointer>
auto toBytes(const BytesConvertiblePointer& data) -> decltype(data->toBytes()) {
  return data->toBytes();
}

inline std::vector<byte> toBytes(const std::string& data) {
  auto bytes = toBytes(static_cast<std::uint16_t>(data.size()));
  bytes.insert(std::end(bytes), std::begin(data), std::end(data));

  return bytes;
}

inline std::vector<byte> toBytes(const boost::string_view& data) {
  auto bytes = toBytes(static_cast<std::uint16_t>(data.size()));
  bytes.insert(std::end(bytes), std::begin(data), std::end(data));

  return bytes;
}

template <class T>
std::vector<byte> toBytes(const std::vector<T>& list) {
  auto bytes = toBytes(static_cast<std::uint16_t>(list.size()));
  for (const auto& element : list) {
    auto elem_bytes = toBytes(element);
    bytes.insert(std::end(bytes), std::begin(elem_bytes), std::end(elem_bytes));
  }

  return bytes;
}

template <class ValueType>
std::vector<byte> toBytes(
    const std::unordered_map<std::string, ValueType>& string_map) {
  auto bytes = toBytes(static_cast<std::uint16_t>(string_map.size()));
  for (const auto& kv : string_map) {
    auto key_bytes = toBytes(kv.first);
    bytes.insert(std::end(bytes), std::begin(key_bytes), std::end(key_bytes));

    auto value_bytes = toBytes(kv.second);
    bytes.insert(std::end(bytes), std::begin(value_bytes),
                 std::end(value_bytes));
  }

  return bytes;
}

std::unordered_map<boost::string_view, boost::string_view> toStringMap(
    const std::vector<byte>& bytes);

boost::string_view toLongString(const std::vector<byte>& bytes);

cpp17::span<byte> toShortBytes(const std::vector<byte>& bytes);

}  // namespace convert
}  // namespace cqlsvrkit
