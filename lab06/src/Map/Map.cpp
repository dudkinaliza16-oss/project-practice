#include "Map.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

bool MapInit(Map& map, const std::string& levelFilePath) {
    if (!map.tilesetTexture.loadFromFile("../assets/tileset.png")) {
        std::cerr << "Error: Failed to load tileset image!" << std::endl;
        return false;
    }

    std::ifstream file(levelFilePath);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open level file: " << levelFilePath << std::endl;
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    file.close();

    if (lines.empty()) {
        std::cerr << "Error: Level file is empty: " << levelFilePath << std::endl;
        return false;
    }

    map.height = static_cast<uint32_t>(lines.size());
    map.width = static_cast<uint32_t>(lines[0].size());
    map.grid = std::vector<TileType>(map.width * map.height, TileType::Empty);

    for (uint32_t y = 0; y < map.height; ++y) {
        uint32_t rowWidth = static_cast<uint32_t>(lines[y].size());
        for (uint32_t x = 0; x < map.width && x < rowWidth; ++x) {
            char c = lines[y][x];
            TileType type = TileType::Empty;

            if (c == '#') {
                type = TileType::Solid;
            } else if (c == 'X') {
                type = TileType::Kill;
            } else if (c == 'W') {
                type = TileType::Victory;
            } else if (c == 'D') {
                type = TileType::Defeat;
            }

            map.grid[y * map.width + x] = type;
        }
    }
    return true;
}

void MapDraw(sf::RenderWindow& window, const Map& map) {
    sf::Sprite tileSprite(map.tilesetTexture);
    tileSprite.setScale({TILE_SCALE, TILE_SCALE});
    for (uint32_t y = 0; y < map.height; ++y) {
        for (uint32_t x = 0; x < map.width; ++x) {
            TileType tile = map.grid[y * map.width + x];
            if (tile == TileType::Empty) {
                continue;
            }
            if (tile == TileType::Solid) {
                tileSprite.setTextureRect(sf::IntRect({16, 0}, {static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE)}));
            } else if (tile == TileType::Kill) {
                tileSprite.setTextureRect(sf::IntRect({192, 48}, {static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE)}));
            } else if (tile == TileType::Victory) {
                tileSprite.setTextureRect(sf::IntRect({48, 16}, {static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE)}));
            } else if (tile == TileType::Defeat) {
                tileSprite.setTextureRect(sf::IntRect({48, 0}, {static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE)}));
            }

            tileSprite.setPosition({x * WORLD_TILE, y * WORLD_TILE});
            window.draw(tileSprite);
        }
    }
}

TileType MapGetTile(const Map& map, float worldX, float worldY) {
    if (worldX < 0 || worldX >= map.width * WORLD_TILE ||
        worldY < 0 || worldY >= map.height * WORLD_TILE) {
        return TileType::Empty;
    }
    int row = static_cast<int>(worldY / WORLD_TILE);
    int col = static_cast<int>(worldX / WORLD_TILE);
    return map.grid[row * map.width + col];
}

bool MapIsSolid(const Map& map, float worldX, float worldY) {
    TileType tile = MapGetTile(map, worldX, worldY);
    return tile == TileType::Solid;
}

std::vector<TileCollisionInfo> MapGetNearbyTiles(const Map& map, const sf::FloatRect& bounds) {
    std::vector<TileCollisionInfo> tiles;

    int minCol = static_cast<int>(std::floor(bounds.position.x / WORLD_TILE));
    int maxCol = static_cast<int>(std::floor((bounds.position.x + bounds.size.x) / WORLD_TILE));
    int minRow = static_cast<int>(std::floor(bounds.position.y / WORLD_TILE));
    int maxRow = static_cast<int>(std::floor((bounds.position.y + bounds.size.y) / WORLD_TILE));

    minCol = std::max(minCol, 0);
    minRow = std::max(minRow, 0);
    maxCol = std::min(maxCol, static_cast<int>(map.width) - 1);
    maxRow = std::min(maxRow, static_cast<int>(map.height) - 1);

    for (int row = minRow; row <= maxRow; ++row) {
        for (int col = minCol; col <= maxCol; ++col) {
            TileType type = map.grid[row * map.width + col];
            if (type != TileType::Empty) {
                tiles.push_back({
                    sf::FloatRect({static_cast<float>(col * WORLD_TILE), static_cast<float>(row * WORLD_TILE)},
                                  {static_cast<float>(WORLD_TILE), static_cast<float>(WORLD_TILE)}),
                    type
                });
            }
        }
    }
    return tiles;
}