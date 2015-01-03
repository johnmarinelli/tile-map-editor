#ifndef TILE_H
#define TILE_H

#include "json/json.h"

#include <SFML/Graphics.hpp>

/*
 * class for physical representation of tile
 */
class Tile
{
private:
    sf::Sprite mSprite;

    /* map coordinates */
    sf::Vector2i mCoords;

    /* index for TileSheetHandler */
    unsigned short mTileSheetIndex;

    /* coordinates on tile sheet */
    sf::Vector2i mTileSheetCoords;

    sf::Vector2i mDimensions;

    bool mTraversable;

public:
    Tile();
    explicit Tile(const sf::Texture& tileSheet,
                  unsigned short tileSheetIndex,
                  const sf::Rect<int>& tileSheetCoords,
                  const sf::Vector2i& coords);

    const sf::Sprite& getSprite() const;
    const sf::Vector2i& getCoords() const;
    const sf::Vector2i& getTileSheetCoords() const;
    const sf::Vector2i& getDimensions() const;
    bool getTraversable() const;
    unsigned short getTileSheetIndex() const;

    Json::Value serialize() const;

    void setCoords(const sf::Vector2i& coords);
    void setTileSheetCoords(const sf::Vector2i& tileSheetCoords);
    void setDimensions(const sf::Vector2i& dimensions);
    void setTraversable(bool traversable);
};

#endif // TILE_H
