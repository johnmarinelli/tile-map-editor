#ifndef TILEMAP_H
#define TILEMAP_H

#include "tile.h"
#include "tilesheethandler.h"

#include <QString>

#include <iostream>
#include <vector>
#include <algorithm>

/*
 * class to hold information pertaining to a 2d tilemap.
 */
class TileMap
{
private:
    int mTileWidth;
    int mTileHeight;
    int mWidth;
    int mHeight;

    std::vector<const Tile*> mTiles;

public:
    TileMap();
    TileMap(int tileWidth,
            int tileHeight,
            int width,
            int height);

    int getTileWidth() const;
    int getTileHeight() const;
    int getWidth() const;
    int getHeight() const;

    void setTileWidth(int tileWidth);
    void setTileHeight(int tileHeight);
    void setWidth(int width);
    void setHeight(int height);

    void addTile(const Tile& tile);
    void removeTile(const Tile* other);

    void saveMap(const QString& path, const TileSheetHandler& tsh) const;

    void reset();
};

#endif // TILEMAP_H
