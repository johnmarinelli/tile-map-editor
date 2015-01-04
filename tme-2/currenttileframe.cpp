#include "currenttileframe.h"
#include "utility.h"

CurrentTileFrame::CurrentTileFrame(QWidget *parent) :
    QFrame(parent),
    mCurrentTileGraphic(new QLabel(this)),
    mTraversableLabel(new QLabel("Traversable", this)),
    mCurrentTileWorldCoords(new QLabel("World Coords", this)),
    mCurrentTileTileSheetCoords(new QLabel("TileSheet Coords:", this)),
    mCurrentTileTraversable(new QComboBox(this))
{
    mCurrentTileTraversable->addItem("True");
    mCurrentTileTraversable->addItem("False");

    /* layout for current tile frame */
    QGridLayout* layout = new QGridLayout(this);
    this->setLayout(layout);
    mCurrentTileGraphic->setFixedSize(128, 128);
    layout->addWidget(mCurrentTileGraphic, 0, 0, 5, 5, Qt::AlignTop);

    /* set coordinate texts & traversable combo box */
    layout->addWidget(mCurrentTileWorldCoords, 2, 0, 5, 5, Qt::AlignLeft);
    layout->addWidget(mCurrentTileTileSheetCoords, 3, 0, 5, 5, Qt::AlignLeft);
    layout->addWidget(mTraversableLabel, 4, 0, 5, 5, Qt::AlignLeft);
    layout->addWidget(mCurrentTileTraversable, 4, 1, 5, 5, Qt::AlignRight);

    /* connect tile information frame to current tile selected */
    connect(mCurrentTileTraversable, SIGNAL(currentIndexChanged(const QString&)),
        parent, SLOT(sendTraversableInformation(const QString&)));
}

void CurrentTileFrame::receiveTileInformation(const Tile& tile, const TileSheetHandler& tsh)
{
    int xOffset = tile.getTileSheetCoords().x;
    int yOffset = tile.getTileSheetCoords().y;
    int tileWidth = tile.getDimensions().x;
    int tileHeight = tile.getDimensions().y;

    int destWidth = mCurrentTileGraphic->geometry().width();
    int destHeight = mCurrentTileGraphic->geometry().height();

    /* clip tilesheet to x, y, tilewidth, tileheight */
    QPixmap gfx = tsh.get(tile.getTileSheetIndex())->getQtTileSheet().copy(xOffset, yOffset, tileWidth, tileHeight)
                    .scaled(QSize(destWidth, destHeight), Qt::IgnoreAspectRatio);

    mCurrentTileGraphic->setPixmap(gfx);

    /* set coordinate texts */
    std::string worldCoordText = "World Position "                   +
                                 std::to_string(tile.getCoords().x)  +
                                 ", "                                +
                                 std::to_string(tile.getCoords().y);

    mCurrentTileWorldCoords->setText(QString(worldCoordText.c_str()));

    std::string tileSheetCoordText = "Tilesheet Position "           +
                                     std::to_string(xOffset)         +
                                     ", "                            +
                                     std::to_string(yOffset);

    mCurrentTileTileSheetCoords->setText(QString(tileSheetCoordText.c_str()));

    /* set if tile is traversable or not */
    int index = mCurrentTileTraversable->findText(boolToString(tile.getTraversable()));
    if(-1 != index) {
        mCurrentTileTraversable->setCurrentIndex(index);
    }
}
