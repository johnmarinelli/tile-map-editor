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
    mSelectionMode(false),
    mSelectedRegion(QRect()),
    mStartX(0),
    mStartY(0)
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

    if(mSelectionMode) {
        sf::RectangleShape selectedRect;
        selectedRect.setSize(sf::Vector2<float>(mSelectedRegion.width(), mSelectedRegion.height()));
        selectedRect.setOutlineColor(sf::Color::Blue);
        selectedRect.setOutlineThickness(5.5f);
        selectedRect.setFillColor(sf::Color(0, 0, 255, 128));
        selectedRect.setPosition(mSelectedRegion.x(), mSelectedRegion.y());

        draw(selectedRect);
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
    if(!mSelectionMode) {
        if(mSelectedTile) mSelectedTile->setTraversable(traversable);
    }
    else {
        for(const auto& tile : mSelectedTiles) {
            tile->setTraversable(traversable);
        }
    }
}

void MyCanvas::setSelectionMode(bool selectionMode)
{
    mSelectionMode = selectionMode;

    /* if selection mode gets turned off, reset the selected region */
    if(!selectionMode) {
        mSelectedRegion.setSize(QSize(0,0));
        mSelectedRegion.setLeft(0);
        mSelectedRegion.setTop(0);
    }
}

void MyCanvas::reset()
{
    mTileMap.reset();
    mGridLines.clear();
    mTiles.clear();
    mSelectedTile = nullptr;
    mTileSheetIndex = 0;
    mCurrentTileBounds = sf::Rect<int>();
    mSelectionMode = false;
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

    if(!mSelectionMode) {
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
    }
    else {
        mStartX = mouseX;
        mStartY = mouseY;
    }

    emit clicked();
}

void MyCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if(!mSelectionMode) {
        addTile(event->pos().x(), event->pos().y());
    }
    else {
        int endX = event->pos().x();
        int endY = event->pos().y();

        mSelectedRegion = getROI(mStartX, mStartY, endX, endY);
    }
}

void MyCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    int endX = event->pos().x();
    int endY = event->pos().y();

    if(mSelectionMode) {
        mSelectedTiles.clear();
        mSelectedRegion = getROI(mStartX, mStartY, endX, endY);

        /* get all tiles within roi */
        for(auto& kv : mTiles) {
            const auto& pos = kv.first;
            auto& tile = kv.second;

            QRect r(pos.x, pos.y, mTileMap.getTileWidth(), mTileMap.getTileHeight());
            if(mSelectedRegion.intersects(r)) {
                mSelectedTiles.push_back(&tile);
            }
        }

        std::cout << mSelectedTiles.size() << std::endl;
    }
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
