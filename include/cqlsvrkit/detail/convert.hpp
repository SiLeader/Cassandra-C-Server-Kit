//
// Created by cerussite on 2/13/20.
//

#pragma once

#include <map>
#include <unordered_map>

#include <endian.h>

#include "../error.hpp"
#include "../types.hpp"

namespace cqlsvrkit {
namespace convert {
namespace detail {

inline void convert_assert(bool expression, const char* message) {
  if (!expression) {
    throw error::ProtocolError(message);
  }
}

template <class Iterator, class Category>
using enable_if_category_t = std::enable_if_t<std::is_same<
    Category,
    typename std::iterator_traits<Iterator>::iterator_category>::value>;

}  // namespace detail

inline std::uint16_t hostToNetwork(std::uint16_t n) { return htobe16(n); }
inline std::uint32_t hostToNetwork(std::uint32_t n) { return htobe32(n); }
inline std::uint64_t hostToNetwork(std::uint64_t n) { return htobe64(n); }
inline std::int16_t hostToNetwork(std::int16_t n) { return htobe16(n); }
inline std::int32_t hostToNetwork(std::int32_t n) { return htobe32(n); }
inline std::int64_t hostToNetwork(std::int64_t n) { return htobe64(n); }

template <class Integral>
Integral networkToHost(Integral n) {
  return hostToNetwork(n);
}

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

std::vector<byte> toBytes(const std::string& data);

std::vector<byte> toBytes(const boost::string_view& data);

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

template <class Integral,
          class = std::enable_if_t<std::is_integral<Integral>::value>>
Integral toInteger(const std::vector<byte>& data,
                   std::size_t* offset = nullptr) {
  auto offset_value = offset == nullptr ? 0 : *offset;
  detail::convert_assert(data.size() >= offset_value,
                         "invalid request body length");

  if (offset) {
    *offset += sizeof(Integral);
  }

  return networkToHost(
      *reinterpret_cast<const Integral*>(data.data() + offset_value));
}

template <class Ret, class Length>
Ret toSequentialDataImpl(const std::vector<byte>& data,
                         std::size_t* offset = nullptr) {
  auto offset_value = offset == nullptr ? 0 : *offset;
  detail::convert_assert(data.size() >= offset_value,
                         "invalid request body length");

  auto length =
      static_cast<std::size_t>(toInteger<Length>(data, &offset_value));
  if (offset) {
    *offset = offset_value + length;
  }

  return Ret{
      reinterpret_cast<typename Ret::const_pointer>(data.data() + offset_value),
      length};
}

boost::string_view toString(const std::vector<byte>& data,
                            std::size_t* offset = nullptr);

boost::string_view toLongString(const std::vector<byte>& data,
                                std::size_t* offset = nullptr);

std::map<boost::string_view, boost::string_view> toStringMap(
    const std::vector<byte>& data, std::size_t* offset = nullptr);

cpp17::span<byte> toShortBytes(const std::vector<byte>& data,
                               std::size_t* offset = nullptr);

cpp17::span<byte> toBytes(const std::vector<byte>& data,
                          std::size_t* offset = nullptr);

std::vector<boost::string_view> toStringList(const std::vector<byte>& data,
                                             std::size_t* offset = nullptr);

template <class T>
T toData(const std::vector<byte>& data, std::size_t* offset = nullptr) {
  return T::Make(data, offset);
}

}  // namespace convert
}  // namespace cqlsvrkit
