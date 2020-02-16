//
// Created by cerussite on 2/16/20.
//

#pragma once

#include <string>
#include <vector>

#include "common.hpp"

namespace cqlsvrkit {

class Supported {
 private:
  std::vector<std::string> versions_;
  std::vector<std::string> compression_;

 public:
  Supported(std::vector<std::string> versions,
            std::vector<std::string> compression)
      : versions_(std::move(versions)), compression_(std::move(compression)) {}

  Supported(const Supported&) = default;
  Supported(Supported&&) = default;

  Supported& operator=(const Supported&) = default;
  Supported& operator=(Supported&&) = default;
  ~Supported() = default;

 public:
  std::vector<byte> toBytes() const;

 public:
  static Supported Make(std::vector<std::string> versions,
                        std::vector<std::string> compression) {
    return Supported(std::move(versions), std::move(compression));
  }
};

}  // namespace cqlsvrkit
