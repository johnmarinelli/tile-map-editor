#include "jpushbutton.h"

JPushButton::JPushButton(QWidget *parent, const TileSheetPtr tileSheetHandle) :
    QPushButton(parent),
    mTileSheetHandle(tileSheetHandle),
    mClipBounds(0, 0, 0, 0)
{
    /* remap our normal clicked() function to our slot, sendTileToDraw */
    connect(this, SIGNAL(clicked()), this, SLOT(sendTileToDraw()));
}

void JPushButton::sendTileToDraw()
{
    emit clicked(mClipBounds, mTileSheetHandle);
}

void JPushButton::setClipSize(int width, int height)
{
    mClipBounds.width = width;
    mClipBounds.height = height;
}

void JPushButton::setClipPosition(int top, int left)
{
    mClipBounds.top = top;
    mClipBounds.left = left;
}

void JPushButton::setClipBounds(int top, int left, int width, int height)
{
    mClipBounds.top = top;
    mClipBounds.left = left;
    mClipBounds.width = width;
    mClipBounds.height = height;
}

sf::Rect<int> JPushButton::getClipBounds()
{
    return mClipBounds;
}
