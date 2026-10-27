#include "Map.h"
#include <iostream>

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
    for (uint32_t x = 5; x <= 12; ++x) {
        map.grid[14 * map.width + x] = TileType::Solid;
    }

    for (uint32_t x = 15; x <= 24; ++x) {
        map.grid[10 * map.width + x] = TileType::Solid;
    }

    for (uint32_t x = 8; x <= 18; ++x) {
        map.grid[6 * map.width + x] = TileType::Solid;
    }

}

void MapDraw(sf::RenderWindow& window, const Map& map) {
    sf::Sprite tileSprite(map.tilesetTexture);
    for (uint32_t y = 0; y < map.height; ++y) {
        for (uint32_t x = 0; x < map.width; ++x) {
            TileType tile = map.grid[y * map.width + x];
            if (tile == TileType::Empty) {
                continue;
            }
            if (tile == TileType::Solid) {
                tileSprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE)}));
            } else if (tile == TileType::Empty) {
                tileSprite.setTextureRect(sf::IntRect({32, 0}, {static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE)}));
            }
            float worldX = static_cast<float>(x * TILE_SIZE);
            float worldY = static_cast<float>(y * TILE_SIZE);

            tileSprite.setPosition({worldX, worldY});
            window.draw(tileSprite);
        }
    }
}

TileType MapGetTile(const Map& map, float worldX, float worldY) {
    if (worldX < 0 || worldX >= map.width*TILE_SIZE || worldY < 0 || worldY >= map.height*TILE_SIZE) {
        return TileType::Empty;
    }
    int row = worldY / TILE_SIZE;
    int col = worldX / TILE_SIZE;
    int index = row * map.width + col;
    return map.grid[index];
}

bool MapIsSolid(const Map& map, float worldX, float worldY) {
    TileType tile = MapGetTile(map, worldX, worldY);
    return tile == TileType::Solid;
}