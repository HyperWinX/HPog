#pragma once

#include <cstdint>
#include <type_traits>

namespace pog {

struct LineSpecialization {
  LineSpecialization(std::uint32_t line, std::uint16_t offset, std::uint16_t length)
    : line(line)
    , offset(offset)
    , length(length) { }
  LineSpecialization()
    : line(1)
    , offset(0)
    , length(0) { }
  
  std::uint32_t line;
  std::uint16_t offset;
  std::uint16_t length;
};

template <typename ValueT>
struct TokenWithLineSpec {
  TokenWithLineSpec() {
    static_assert(std::is_default_constructible_v<ValueT>, "ValueT should be default constructible");
  }
  TokenWithLineSpec(TokenWithLineSpec<ValueT>&) = default;
  TokenWithLineSpec(TokenWithLineSpec<ValueT>&&) = default;
  TokenWithLineSpec<ValueT>& operator=(TokenWithLineSpec<ValueT>&) = default;

  ValueT value;
  LineSpecialization* line_spec;
};

}