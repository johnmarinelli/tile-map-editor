#include "jradiobutton.h"

JRadioButton::JRadioButton(const QString& text, sf::Texture& tilesheet, QWidget *parent) :
    QRadioButton(text, parent),
    mTilesheet(tilesheet),
    mClipBounds(0, 0, 0, 0)
{
}

void JRadioButton::setIconPath(const QString path)
{
    this->mIconPath = path;
    qDebug(mIconPath.toUtf8().constData());
}

void JRadioButton::setClipBounds(int top, int left, int width, int height)
{
    mClipBounds.top = top;
    mClipBounds.left = left;
    mClipBounds.width = width;
    mClipBounds.height = height;
}

sf::Rect<int> JRadioButton::getClipBounds()
{
    return mClipBounds;
}
