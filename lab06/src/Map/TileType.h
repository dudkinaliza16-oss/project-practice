#pragma once
#include <cstdint>

enum class TileType : uint8_t {
    Empty = 0,
    Solid = 1,
    Kill = 2
};