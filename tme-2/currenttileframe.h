#ifndef CURRENTTILEFRAME_H
#define CURRENTTILEFRAME_H

#include <QFrame>
#include <QLabel>
#include <QComboBox>

#include "tile.h"
#include "tilesheethandler.h"

class CurrentTileFrame : public QFrame
{
private:
    QLabel* mCurrentTileGraphic;
    QLabel* mTraversableLabel;
    QLabel* mCurrentTileWorldCoords;
    QLabel* mCurrentTileTileSheetCoords;

    QComboBox* mCurrentTileTraversable;

    Q_OBJECT

public:
    explicit CurrentTileFrame(QWidget* parent = 0);

    void receiveTileInformation(const Tile& tile, const TileSheetHandler& tsh);

signals:

public slots:

};

#endif // CURRENTTILEFRAME_H
