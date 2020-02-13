//
// Created by cerussite on 2/13/20.
//

#include <cqlsvrkit/detail/convert.hpp>
#include <cqlsvrkit/types.hpp>

namespace cqlsvrkit {

Supported::Supported(std::vector<std::string> versions,
                     std::vector<std::string> compression)
    : versions_(std::move(versions)), compression_(std::move(compression)) {}

std::vector<byte> Supported::toBytes() const {
  std::unordered_map<std::string, std::vector<std::string>> data;
  data["CQL_VERSION"] = versions_;
  data["COMPRESSION"] = compression_;

  return convert::toBytes(data);
}

}  // namespace cqlsvrkit