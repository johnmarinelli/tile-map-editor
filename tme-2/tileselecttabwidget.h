#ifndef TILESELECTTABWIDGET_H
#define TILESELECTTABWIDGET_H

#include "tilesheethandler.h"
#include "mycanvas.h"

#include <QScrollArea>
#include <QTabWidget>

class TileSelectTabWidget : public QTabWidget
{
private:
    int mCurrentTabIndex;

    void setTabLayout(QScrollArea* scrollArea, QWidget* scrollAreaContents, const std::shared_ptr<const TileSheet>& tileSheet, MyCanvas& canvas);

    Q_OBJECT
public:
    explicit TileSelectTabWidget(QWidget* parent = 0);
    TileSelectTabWidget(int windowWidth, int windowHeight, QWidget* parent = 0);

    int getCurrentTabIndex() const;
    void setTabs(TileSheetHandler& tsh, MyCanvas& canvas);

signals:

public slots:

private slots:
    void setCurrentTabIndex(int index);

};

#endif // TILESELECTTABWIDGET_H
