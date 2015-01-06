#ifndef CANVASFRAME_H
#define CANVASFRAME_H

#include "mycanvas.h"
#include "tilesheethandler.h"
#include <QFrame>
#include <QScrollArea>

/*
 * frame to hold scroll area, and cavas
 */
class CanvasFrame : public QFrame
{
private:
    QScrollArea* mScrollArea;
    QWidget* mScrollAreaWidget;
    MyCanvas* mCanvas;

    void setScrollAreaLayout();
    void resizeScrollArea();
    void initCanvas(const QPoint& pos, const QSize& size,
                    const TileSheetHandler& tsh, int tileWidth,
                    int tileHeight);

    Q_OBJECT
public:
    explicit CanvasFrame(QWidget *parent = 0);

    void init(const QPoint& pos, const QSize& size,
              const TileSheetHandler& tsh, int tileWidth,
              int tileHeight);

    MyCanvas* getCanvas() const;

    ~CanvasFrame();

signals:

public slots:

};

#endif // CANVASFRAME_H
