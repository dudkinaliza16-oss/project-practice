#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "TileType.h"

constexpr uint32_t TILE_SIZE = 16;
constexpr float TILE_SCALE = 3.f;
constexpr float WORLD_TILE = TILE_SIZE * TILE_SCALE;

struct Map {
    std::vector<TileType> grid;
    uint32_t width = 0;
    uint32_t height = 0;
    sf::Texture tilesetTexture;
};

void MapInit(Map& map);
void MapDraw(sf::RenderWindow& window, const Map& map);
TileType MapGetTile(const Map& map, float worldX, float worldY);
bool MapIsSolid(const Map& map, float worldX, float worldY);
std::vector<sf::FloatRect> MapGetNearbyTiles(const Map& map, const sf::FloatRect& bounds);