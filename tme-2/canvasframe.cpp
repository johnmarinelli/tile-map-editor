#include "canvasframe.h"
#include "utility.h"
#include <iostream>

CanvasFrame::CanvasFrame(QWidget *parent) :
    QFrame(parent),
    mScrollArea(new QScrollArea(this)),
    mScrollAreaWidget(new QWidget),
    mCanvas(nullptr)
{
    mScrollAreaWidget->setParent(mScrollArea);
    mScrollArea->setWidget(mScrollAreaWidget);

    this->setObjectName("SFMLFrame");
    mScrollArea->setObjectName("SFMLScrollArea");
    mScrollAreaWidget->setObjectName("SFMLScrollAreaWidget");
}

void CanvasFrame::initCanvas(const QPoint &pos, const QSize &size, const TileSheetHandler &tsh, int tileWidth, int tileHeight)
{
    mCanvas = new MyCanvas(mScrollAreaWidget, pos, size, tsh, tileWidth, tileHeight);
    mCanvas->setObjectName("canvas");
}

void CanvasFrame::setScrollAreaLayout()
{
    mScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    removeLayout(mScrollAreaWidget->layout());

    QGridLayout* layout = new QGridLayout(mScrollAreaWidget);
    mScrollAreaWidget->setLayout(layout);
    mScrollArea->setWidgetResizable(false);

    /* add canvas to scroll area widget */
    if(nullptr != mCanvas) layout->addWidget(mCanvas);
    else std::cerr << "CanvasFrame::setScrollAreaLayout: mCanvas must not be null!" << std::endl;
}

void CanvasFrame::resizeScrollArea()
{
    QRect frameRect = this->geometry();

    /* size of actual scroll area */
    mScrollArea->resize(frameRect.width(), frameRect.height());
    mScrollArea->setStyleSheet("background-color: red");

    /* size of scroll area contents; determines scroll bars */
    mScrollAreaWidget->resize(mCanvas->size());
    mScrollAreaWidget->setStyleSheet("background-color:orange");

}

void CanvasFrame::init(const QPoint &pos, const QSize &size, const TileSheetHandler &tsh, int tileWidth, int tileHeight)
{
    initCanvas(pos, size, tsh, tileWidth, tileHeight);
    setScrollAreaLayout();
    resizeScrollArea();
}

MyCanvas* CanvasFrame::getCanvas() const
{
    return mCanvas;
}

CanvasFrame::~CanvasFrame()
{
    mCanvas->reset();
}
