//
// Created by cerussite on 2/16/20.
//

#include <cqlsvrkit/detail/convert.hpp>
#include <cqlsvrkit/types/query.hpp>

namespace cqlsvrkit {

QueryParameter QueryParameter::Make(const std::vector<tcp::byte>& body,
                                    std::size_t* offset) {
  auto offset_value = offset == nullptr ? 0 : *offset;
  convert::detail::convert_assert(body.size() >= offset_value,
                                  "invalid request body length");

  auto consistency = static_cast<Consistency>(
      convert::toInteger<std::uint16_t>(body, &offset_value));
  auto flags = convert::toInteger<std::uint8_t>(body, &offset_value);

  auto has_values = QueryParameter::CheckFlag(flags, Flags::kValues);
  auto skip_metadata = QueryParameter::CheckFlag(flags, Flags::kSkipMetadata);
  auto has_page_size = QueryParameter::CheckFlag(flags, Flags::kPageSize);
  auto with_paging_state =
      QueryParameter::CheckFlag(flags, Flags::kWithPagingState);
  auto with_serial_consistency =
      QueryParameter::CheckFlag(flags, Flags::kWithSerialConsistency);
  auto with_default_timestamp =
      QueryParameter::CheckFlag(flags, Flags::kWithDefaultTimestamp);
  auto with_names_for_values =
      QueryParameter::CheckFlag(flags, Flags::kWithNamesForValues);

  boost::variant<std::vector<Value>, std::map<boost::string_view, Value>>
      values;
  if (has_values) {
    auto length = convert::toInteger<std::int16_t>(body, &offset_value);

    if (with_names_for_values) {
      std::map<boost::string_view, Value> vals;
      for (int i = 0; i < length; ++i) {
        auto name = convert::toString(body, &offset_value);
        auto value = convert::toData<Value>(body, &offset_value);
        vals[name] = value;
      }
      values = vals;

    } else {
      std::vector<Value> vals;
      for (int i = 0; i < length; ++i) {
        auto value = convert::toData<Value>(body, &offset_value);
        vals.emplace_back(value);
      }
      values = vals;
    }
  }

  std::int32_t result_page_size = 0;
  if (has_page_size) {
    result_page_size = convert::toInteger<std::int32_t>(body, &offset_value);
  }

  cpp17::span<byte> paging_state;
  if (with_paging_state) {
    paging_state = convert::toBytes(body, &offset_value);
  }

  Consistency serial_consistency = Consistency::kSerial;
  if (with_serial_consistency) {
    serial_consistency = static_cast<Consistency>(
        convert::toInteger<std::uint16_t>(body, &offset_value));
  }

  std::int64_t timestamp = 0;
  if (with_default_timestamp) {
    timestamp = convert::toInteger<std::int64_t>(body, &offset_value);
  }

  if (offset) {
    *offset = offset_value;
  }

  return {
      consistency,
      flags,
      values,
      skip_metadata,
      result_page_size,
      paging_state,
      serial_consistency,
      timestamp,
  };
}

}  // namespace cqlsvrkit
