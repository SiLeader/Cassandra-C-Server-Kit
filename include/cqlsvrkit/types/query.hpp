//
// Created by cerussite on 2/16/20.
//

#pragma once

#include <map>
#include <variant>
#include <vector>

#include <boost/variant.hpp>

#include "common.hpp"
#include "header.hpp"
#include "value.hpp"

namespace cqlsvrkit {

enum class Flags : std::uint8_t {
  kValues = 0x01,
  kSkipMetadata = 0x02,
  kPageSize = 0x04,
  kWithPagingState = 0x08,
  kWithSerialConsistency = 0x10,
  kWithDefaultTimestamp = 0x20,
  kWithNamesForValues = 0x40,
};

class QueryParameter {
 private:
  Consistency consistency_;
  std::uint8_t flags_;
  boost::variant<std::vector<Value>, std::map<boost::string_view, Value>>
      values_;
  std::int32_t result_page_size_;
  cpp17::span<byte> paging_state_;
  Consistency serial_consistency_;
  std::int64_t timestamp_;
  bool skip_metadata_;

 public:
  QueryParameter(
      Consistency consistency, std::uint8_t flags,
      boost::variant<std::vector<Value>, std::map<boost::string_view, Value>>
          values,
      bool skip_metadata, std::int32_t result_page_state,
      cpp17::span<byte> paging_state, Consistency serial_consistency,
      std::int64_t timestamp)
      : consistency_(consistency),
        flags_(flags),
        values_(std::move(values)),
        result_page_size_(result_page_state),
        paging_state_(paging_state),
        serial_consistency_(serial_consistency),
        timestamp_(timestamp),
        skip_metadata_(skip_metadata) {}

 public:
  static QueryParameter Make(const std::vector<tcp::byte>& body,
                             std::size_t* offset = nullptr);

 private:
  static bool CheckFlag(std::uint8_t flags, Flags flag) noexcept {
    return (flags & static_cast<std::uint8_t>(flag)) != 0;
  }

 public:
  Consistency consistency() const noexcept { return consistency_; }
  std::uint8_t flags() const noexcept { return flags_; }

 public:
  bool hasValues() const noexcept { return CheckFlag(flags(), Flags::kValues); }
  bool isValueWithName() const noexcept {
    return CheckFlag(flags(), Flags::kWithNamesForValues);
  }
  const boost::variant<std::vector<Value>, std::map<boost::string_view, Value>>&
  values() const {
    return values_;
  }

 public:
  bool hasResultPageSize() const noexcept {
    return CheckFlag(flags(), Flags::kPageSize);
  }
  std::int32_t resultPageSize() const noexcept { return result_page_size_; }

 public:
  bool hasPagingState() const noexcept {
    return CheckFlag(flags(), Flags::kWithPagingState);
  }
  const cpp17::span<byte>& pagingState() const noexcept {
    return paging_state_;
  }

 public:
  bool hasSerialConsistency() const noexcept {
    return CheckFlag(flags(), Flags::kWithSerialConsistency);
  }
  Consistency serialConsistency() const noexcept { return serial_consistency_; }

 public:
  bool hasDefaultTimestamp() const noexcept {
    return CheckFlag(flags(), Flags::kWithDefaultTimestamp);
  }
  std::int64_t defaultTimestamp() const noexcept { return timestamp_; }

 public:
  bool isSkipMetadata() const noexcept { return skip_metadata_; }
};

}  // namespace cqlsvrkit
