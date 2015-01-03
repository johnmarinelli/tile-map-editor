#include "tilesheet.h"
#include <iostream>

TileSheet::TileSheet() :
    mSize(0, 0),
    mRows(0),
    mColumns(0)
{
}

TileSheet::TileSheet(const QString& path, const QSize& tileSize) :
    mPath(path.toStdString()),
    mQtTileSheet(path),
    mSize(0, 0),
    mTileSize(tileSize),
    mRows(0),
    mColumns(0)
{
    mSfTileSheet.loadFromFile(path.toStdString());
    initTileSheet();
}

void TileSheet::setQtTileSheet(const QString& path)
{
    mQtTileSheet.load(path);
}

void TileSheet::setSfTileSheet(const std::string& path)
{
    mSfTileSheet.loadFromFile(path);
}

void TileSheet::initTileSheet()
{
    if(!mQtTileSheet.isNull()) {
        mColumns = std::floor(mQtTileSheet.width() / mTileSize.width());
        mRows = std::floor(mQtTileSheet.height() / mTileSize.height());

        mSize.setHeight(mQtTileSheet.height());
        mSize.setWidth(mQtTileSheet.width());
    }
    else {
        std::cout << "ERROR: TileSheet::initTileSheet: mQtTileSheet is null";
    }
}

const QPixmap& TileSheet::getQtTileSheet() const
{
    return mQtTileSheet;
}

const sf::Texture& TileSheet::getSfTileSheet() const
{
    return mSfTileSheet;
}

int TileSheet::getColumns() const
{
    return mColumns;
}

int TileSheet::getRows() const
{
    return mRows;
}

std::string TileSheet::getPath() const
{
    return mPath;
}
