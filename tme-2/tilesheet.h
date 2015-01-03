#ifndef TILESHEET_H
#define TILESHEET_H

#include <QPixmap>
#include "SFML/Graphics.hpp"

/*
 * This class holds Qt & SFML information about a tilesheet.
 */
class TileSheet
{
private:
    std::string mPath;

    QPixmap mQtTileSheet;
    sf::Texture mSfTileSheet;

    QSize mSize;
    QSize mTileSize;

    int mRows;
    int mColumns;

public:
    TileSheet();
    TileSheet(const QString& qtPath,
              const QSize& tileSize);

    void setQtTileSheet(const QString& path);
    void setSfTileSheet(const std::string& path);

    const QPixmap& getQtTileSheet() const;
    const sf::Texture& getSfTileSheet() const;
    int getRows() const;
    int getColumns() const;

    std::string getPath() const;

    /*
     * Sets size, rows, and columns from tilesheet
     */
    void initTileSheet();
};

#endif // TILESHEET_H
