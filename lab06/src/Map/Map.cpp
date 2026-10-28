#include "Map.h"
#include <cmath>
#include <algorithm>
#include <iostream>

static void AddPlatform(Map& map, uint32_t row, uint32_t fromX, uint32_t toX) {
    for (uint32_t x = fromX; x <= toX; ++x) {
        map.grid[row * map.width + x] = TileType::Solid;
    }
}
void MapInit(Map& map) {
    if (!map.tilesetTexture.loadFromFile("../assets/tileset.png")) {
        std::cerr << "Failed to load image!" << std::endl;
        return;
    }
    map.width = 30;
    map.height = 20;
    map.grid = std::vector<TileType>(map.width * map.height, TileType::Empty);
    for (uint32_t y = 0; y < map.height; ++y) {
        for (uint32_t x = 0; x < map.width; ++x) {
            if (x == 0 || x == map.width - 1 || y == 0 || y == map.height - 1) {
                map.grid[y * map.width + x] = TileType::Solid;
            }
        }
    }
    AddPlatform(map, 17, 3, 7);
    AddPlatform(map, 15, 10, 14);
    AddPlatform(map, 13, 17, 21);
    AddPlatform(map, 11, 23, 27);
    AddPlatform(map, 9, 16, 20);
    AddPlatform(map, 7, 9, 13);
    AddPlatform(map, 5, 3, 7);
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
                tileSprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE)}));
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

std::vector<sf::FloatRect> MapGetNearbyTiles(const Map& map, const sf::FloatRect& bounds) {
    std::vector<sf::FloatRect> tiles;

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
            if (map.grid[row * map.width + col] == TileType::Solid) {
                tiles.emplace_back(sf::Vector2f{col * WORLD_TILE, row * WORLD_TILE},
                                   sf::Vector2f{WORLD_TILE, WORLD_TILE});
            }
        }
    }
    return tiles;
}