//
// Created by cerussite on 2/16/20.
//

#pragma once

#include <cstdint>
#include <vector>

#include "../common.hpp"

namespace cqlsvrkit {

class Rows {
 public:
  class Metadata {
   public:
    enum Flags : std::int32_t {
      kGlobalTablesSpec = 0x0001,
      kHasMorePages = 0x0002,
      kNoMetadata = 0x0003,
    };

    class Column {
     public:
      enum class Type {
        kCustom = 0x0000,
        kAscii = 0x0001,
        kBigint = 0x0002,
        kBlob = 0x0003,
        kBoolean = 0x0004,
        kCounter = 0x0005,
        kDecimal = 0x0006,
        kDouble = 0x0007,
        kFloat = 0x0008,
        kInt = 0x0009,
        kTimestamp = 0x000b,
        kUuid = 0x000c,
        kVarchar = 0x000d,
        kVarint = 0x000e,
        kTimeUuid = 0x000f,
        kInternet = 0x0010,
        kDate = 0x0011,
        kTime = 0x0012,
        kSmallint = 0x0013,
        kTinyint = 0x0014,
        kList = 0x0020,
        kMap = 0x0021,
        kSet = 0x0022,
        kUdt = 0x0030,
        kTuple = 0x0031,
      };

     private:
      std::string keyspace_name_, table_name_;
      std::string name_;
    };

   private:
    std::int32_t flags_;
    std::int32_t columns_count_;
    std::vector<byte> paging_state_;
    std::string global_table_spec_;
  };
};

}  // namespace cqlsvrkit
