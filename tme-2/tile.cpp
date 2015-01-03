#include "tile.h"
#include <iostream>

Tile::Tile()
    : mTileSheetIndex(0),
      mTraversable(true)
{
    std::cout << "Tile empty ctor" << std::endl;
}

Tile::Tile(const sf::Texture& tileSheet,
           unsigned short tileSheetIndex,
           const sf::Rect<int>& tileSheetBounds,
           const sf::Vector2i& coords)
    : mSprite(tileSheet, tileSheetBounds),
      mCoords(coords),
      mTileSheetIndex(tileSheetIndex),
      mTileSheetCoords(tileSheetBounds.left, tileSheetBounds.top),
      mTraversable(true)
{
    mSprite.setPosition(coords.x, coords.y);
}

const sf::Sprite& Tile::getSprite() const
{
    return mSprite;
}

const sf::Vector2i& Tile::getCoords() const
{
    return mCoords;
}

const sf::Vector2i& Tile::getTileSheetCoords() const
{
    return mTileSheetCoords;
}

const sf::Vector2i& Tile::getDimensions() const
{
    return mDimensions;
}

bool Tile::getTraversable() const
{
    return mTraversable;
}

unsigned short Tile::getTileSheetIndex() const
{
    return mTileSheetIndex;
}

Json::Value Tile::serialize() const
{
    Json::Value tile;

    /* prepare json values */
    Json::Value worldCoordsJSON = Json::Value(Json::arrayValue);
    Json::Value tileSheetCoordsJSON = Json::Value(Json::arrayValue);
    Json::Value traversableJSON = mTraversable;

    /* put raw values in json values */
    worldCoordsJSON.append(mCoords.x);
    worldCoordsJSON.append(mCoords.y);

    tileSheetCoordsJSON.append(mTileSheetCoords.x);
    tileSheetCoordsJSON.append(mTileSheetCoords.y);

    /* add JSON values to tile */
    tile["worldCoords"] = worldCoordsJSON;
    tile["tileSheetCoords"] = tileSheetCoordsJSON;
    tile["isTraversable"] = traversableJSON;

    return tile;
}

void Tile::setCoords(const sf::Vector2i& coords)
{
    mCoords.x = coords.x;
    mCoords.y = coords.y;
}

void Tile::setTileSheetCoords(const sf::Vector2i& tileSheetCoords)
{
    mTileSheetCoords.x = tileSheetCoords.x;
    mTileSheetCoords.y = tileSheetCoords.y;
}

void Tile::setDimensions(const sf::Vector2i& dimensions)
{
    mDimensions = dimensions;
}

void Tile::setTraversable(bool traversable)
{
    mTraversable = traversable;
}
