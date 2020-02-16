//
// Created by cerussite on 2/16/20.
//

#include <unordered_map>

#include <cqlsvrkit/detail/convert.hpp>
#include <cqlsvrkit/types/supported.hpp>

namespace cqlsvrkit {

std::vector<byte> Supported::toBytes() const {
  std::unordered_map<std::string, std::vector<std::string>> data;
  data["CQL_VERSION"] = versions_;
  data["COMPRESSION"] = compression_;

  return convert::toBytes(data);
}

}  // namespace cqlsvrkit
