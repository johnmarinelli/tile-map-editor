#include "tilemap.h"

#include "json/json.h"

#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QFileInfo>

TileMap::TileMap()
{
}

TileMap::TileMap(int tileWidth,
                 int tileHeight,
                 int width,
                 int height) :
    mTileWidth(tileWidth),
    mTileHeight(tileHeight),
    mWidth(width),
    mHeight(height)
{
}

int TileMap::getTileWidth() const
{
    return mTileWidth;
}

int TileMap::getTileHeight() const
{
    return mTileHeight;
}

int TileMap::getWidth() const
{
    return mWidth;
}

int TileMap::getHeight() const
{
    return mHeight;
}

void TileMap::setTileWidth(int tileWidth)
{
    mTileWidth = tileWidth;
}

void TileMap::setTileHeight(int tileHeight)
{
    mTileHeight = tileHeight;
}

void TileMap::setWidth(int width)
{
    mWidth = width;
}

void TileMap::setHeight(int height)
{
    mHeight = height;
}

void TileMap::addTile(const Tile &tile)
{
    mTiles.push_back(&tile);
}

void TileMap::removeTile(const Tile *other)
{
    const Tile* remove = nullptr;
    for(const auto& t : mTiles) {
        if(t == other) {
            remove = t;
        }
    }
    mTiles.erase(std::remove(mTiles.begin(), mTiles.end(), remove));
}

void TileMap::saveMap(const QString& path, const TileSheetHandler& tsh) const
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text )) {
        std::cout << "Unable to open " << path.toStdString() << " for writing. " << std::endl;
        return;
    }

    QTextStream output(&file);

    /* root level for tile map */
    Json::Value tileMapRoot;

    /* add dimension info to root */
    tileMapRoot["tileWidth"] = mTileWidth;
    tileMapRoot["tileHeight"] = mTileHeight;
    tileMapRoot["width"] = mWidth;
    tileMapRoot["height"] = mHeight;

    /* nested value for tiles */
    Json::Value tiles = Json::Value(Json::arrayValue);

    for(const auto& t : mTiles) {
        Json::Value tile = t->serialize();

        /* retrieve path of tile sheet for this tile */
        std::string tileSheetPath = tsh.get(t->getTileSheetIndex())->getPath();

        /* create a QFile for easier access to file name */
        QFileInfo fileInfo(tileSheetPath.c_str());
        tileSheetPath = fileInfo.fileName().toStdString();

        std::cout << tileSheetPath;

        /* store filename in tile json */
        tile["tileSheetFile"] = tileSheetPath;
        tiles.append(tile);
    }

    tileMapRoot["tiles"] = tiles;

    /* write to file */
    Json::StyledWriter writer;
    std::string JSONOutput = writer.write(tileMapRoot);

    output << JSONOutput.c_str();
}

void TileMap::reset()
{
    mTileWidth = 0;
    mTileHeight = 0;
    mWidth = 0;
    mHeight = 0;
    mTiles.clear();
}
