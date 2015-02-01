#include "mycanvas.h"
#include "utility.h"

#include <iostream>
#include <sstream>

MyCanvas::MyCanvas(QWidget *parent,
                   const QPoint& position,
                   const QSize& size,
                   const TileSheetHandler& tileSheetHandler,
                   const int tileWidth,
                   const int tileHeight) :
    QSFMLCanvas(parent, position, size),
    mTileSheetHandler(tileSheetHandler),
    mTileSheetIndex(0),
    mSelectedTile(nullptr),
    mTileMap(tileWidth, tileHeight, size.width(), size.height()),
    mSelectionMode(false)
{
    setLines();

    /* remap our normal clicked() function to our slot, sendSelectedTile */
    connect(this, SIGNAL(clicked()), this, SLOT(sendSelectedTile()));
}

sf::Vector2i MyCanvas::getCoordsFromMouse(int mouseX, int mouseY)
{
    int tileWidth = mTileMap.getTileWidth();
    int tileHeight = mTileMap.getTileHeight();

    int x = getNearestMultiple(mouseX, tileWidth);
    int y = getNearestMultiple(mouseY, tileHeight);

    return sf::Vector2i(x, y);
}

void MyCanvas::addTile(int mouseX, int mouseY)
{
    int tileWidth = mTileMap.getTileWidth();
    int tileHeight = mTileMap.getTileHeight();

    sf::Vector2i coords = getCoordsFromMouse(mouseX, mouseY);

    /* place tile at snapped point */
    Tile tile(mTileSheetHandler.get(mTileSheetIndex)->getSfTileSheet(),
              mTileSheetIndex,
              mCurrentTileBounds,
              coords);

    tile.setTileSheetCoords(sf::Vector2i(mCurrentTileBounds.left, mCurrentTileBounds.top));
    tile.setDimensions(sf::Vector2i(tileWidth, tileHeight));

    //std::cout << "assignment operator" << std::endl;
    mTiles[coords] = tile;
    mSelectedTile = &(mTiles[coords]);
    mTileMap.addTile(mTiles[coords]);
}

void MyCanvas::setLines()
{
    int columns = std::ceil(this->size().width() / mTileMap.getTileWidth());
    int rows = std::ceil(this->size().height() / mTileMap.getTileHeight());

    QSize size = this->size();

    /* vertical lines */
    for(int i = 0; i < columns; ++i) {
        int x = i * mTileMap.getTileHeight();
        Line line(x, 0, x, size.height());
        line.mLine[0].color = sf::Color::Black;
        line.mLine[1].color = sf::Color::Black;
        mGridLines.push_back(line);
    }

    /* horizontal lines */
    for(int j = 0; j < rows; ++j) {
        int y = j * mTileMap.getTileWidth();
        Line line(0, y, size.width(), y);
        line.mLine[0].color = sf::Color::Black;
        line.mLine[1].color = sf::Color::Black;
        mGridLines.push_back(line);
    }
}

void MyCanvas::sendSelectedTile()
{
    /* then sendSelectedTile() will emit a different signal that passes an argument */
    if(mSelectedTile) emit clicked(*mSelectedTile);
}

void MyCanvas::onInit()
{
}

void MyCanvas::onUpdate()
{
    sf::RenderWindow::clear(sf::Color(255, 255, 255));

    for(const auto& line : mGridLines) {
        draw(line.mLine, 2, sf::Lines);
    }

    for(const auto& tile : mTiles) {
        /* for some reason using Qt5, this breaks */
        draw(tile.second.getSprite());
    }
}

void MyCanvas::setCurrentTile(const sf::Rect<int>& bounds, const std::shared_ptr<const TileSheet> tileSheet)
{
    mCurrentTileBounds.top = bounds.top;
    mCurrentTileBounds.left = bounds.left;
    mCurrentTileBounds.width = mTileMap.getTileWidth();
    mCurrentTileBounds.height = mTileMap.getTileHeight();
    mTilesheet = tileSheet.get()->getSfTileSheet();
}

void MyCanvas::setCurrentTileTraversable(bool traversable)
{
    if(mSelectedTile) mSelectedTile->setTraversable(traversable);
}

void MyCanvas::setSelectionMode(bool selectionMode)
{
    mSelectionMode = selectionMode;
}

void MyCanvas::reset()
{
    mTileMap.reset();
    mGridLines.clear();
    mTiles.clear();
    mSelectedTile = nullptr;
    mTileSheetIndex = 0;
    mCurrentTileBounds = sf::Rect<int>();
    this->move(0, 0);
    this->resize(0, 0);
}

void MyCanvas::setTileDimensions(int tileWidth, int tileHeight)
{
    mTileMap.setTileWidth(tileWidth);
    mTileMap.setTileHeight(tileHeight);
}

void MyCanvas::setDimensions(int width, int height)
{
    mTileMap.setWidth(width);
    mTileMap.setHeight(height);
    this->resize(width, height);
}

void MyCanvas::saveMap(const QString& path)
{
    mTileMap.saveMap(path, mTileSheetHandler);
}

void MyCanvas::setCurrentTileSheetIndex(int index)
{
    mTileSheetIndex = index;
}

void MyCanvas::mousePressEvent(QMouseEvent* event)
{
    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    if(event->button() & Qt::LeftButton) {
        addTile(mouseX, mouseY);
    }

    else if(event->button() & Qt::RightButton) {
        sf::Vector2i coords = getCoordsFromMouse(mouseX, mouseY);

        /* find tile */
        auto tile = mTiles.find(coords);

        /* select tile */
        if(tile != mTiles.end()) {
            mSelectedTile = &(tile->second);
            //mTileMap.removeTile(&(tile->second));
            //mTiles.erase(tile);
        }
    }

    emit clicked();
}

void MyCanvas::mouseMoveEvent(QMouseEvent *event)
{
    addTile(event->pos().x(), event->pos().y());
}

const TileMap& MyCanvas::getTileMap() const
{
    return mTileMap;
}

QSize MyCanvas::sizeHint() const
{
    return QSize(mTileMap.getWidth(), mTileMap.getHeight());
}

MyCanvas::~MyCanvas()
{
    reset();
}
