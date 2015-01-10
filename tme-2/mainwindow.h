#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mycanvas.h"
#include "jpushbutton.h"
#include "tile.h"
#include "currenttileframe.h"
#include "canvasframe.h"
#include "tileselecttabwidget.h"
#include "utility.h"

#include <QMainWindow>
#include <QRadioButton>
#include <QFrame>
#include <QSignalMapper>
#include <QScrollArea>
#include <QGridLayout>

#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void setTileSelectLayout(QScrollArea* scrollArea, QWidget* scrollAreaContents, const std::shared_ptr<const TileSheet>& tileSheet);

    void resizeCurrentTileFrame();
    void resizeTileSelect();
    void resizeSFMLFrame(QFrame* frame);

    ~MainWindow();

private slots:
    void sendTileInformation(const Tile& tile);
    void sendTraversableInformation(const QString& str);
    void setCurrentTileSheetIndex(int index);
    void showNewMapDialog();
    void makeNewMap(std::tuple<int, int, int, int> params);
    void saveMap();

private:
    void setTileSheetTabs();
    void setTileSelectButtons();
    void createNewCanvasArea(int width=0, int height=0, int tileWidth=DEFAULT_TILE_WIDTH, int tileHeight=DEFAULT_TILE_HEIGHT);

    Ui::MainWindow *ui;

    QGridLayout* mLayout;

    QFrame* mInputFrame;
    CurrentTileFrame* mCurrentTileFrame;
    CanvasFrame* mCanvasFrame;
    TileSelectTabWidget* mTileSelector;

    TileSheetHandler mTileSheetHandler;
    unsigned short mTileSheetIndex;

    int mWindowWidth;
    int mWindowHeight;
};

#endif // MAINWINDOW_H
