#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newmapdialog.h"
#include "utility.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDesktopWidget>
#include <QDir>
#include <QString>
#include <QDirIterator>
#include <QFileDialog>

#include <iostream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mLayout(new QGridLayout),
    mCurrentTileFrame(nullptr),
    mCanvasFrame(nullptr),
    mTileSelector(nullptr),
    mTileSheetIndex(0),
    mWindowWidth(0),
    mWindowHeight(0)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    mWindowWidth = QApplication::desktop()->availableGeometry().width();
    mWindowHeight = QApplication::desktop()->availableGeometry().height();

    this->setGeometry(0, 0, mWindowWidth, mWindowHeight);

    ui->centralwidget->setLayout(mLayout);

    mTileSelector = new TileSelectTabWidget(mWindowWidth, mWindowHeight, ui->centralwidget);

    createNewCanvasArea();
    mCurrentTileFrame = new CurrentTileFrame(this);
    resizeCurrentTileFrame();

    mTileSelector->setTabs(mTileSheetHandler, *mCanvasFrame->getCanvas());

    mLayout->addWidget(mCurrentTileFrame, 0, 1, 1, 1);
    mLayout->addWidget(mTileSelector, 1, 1, 1, 1);

    mLayout->setColumnStretch(1, 30);
    mLayout->setColumnStretch(2, 70);

    /* connect canvas to tile information frame */
    connect(mCanvasFrame->getCanvas(), SIGNAL(clicked(const Tile&)), this, SLOT(sendTileInformation(const Tile&)));

    /* connect tab switching to change mTileSheet */
    connect(mTileSelector, SIGNAL(currentChanged(int)), mCanvasFrame->getCanvas(), SLOT(setCurrentTileSheetIndex(int)));
    connect(mTileSelector, SIGNAL(currentChanged(int)), this, SLOT(setCurrentTileSheetIndex(int)));

    /* connect new file option to new map dialog */
    connect(ui->newMapAction, SIGNAL(triggered()), this, SLOT(showNewMapDialog()));

    /* connect save map */
    connect(ui->saveMapAction, SIGNAL(triggered()), this, SLOT(saveMap()));

    /* connect selection mode to canvas */
    connect(ui->actionSelectionMode, SIGNAL(toggled(bool)), mCanvasFrame->getCanvas(), SLOT(setSelectionMode(bool)));
}

void MainWindow::setTileSheetTabs()
{
    /* add all tilesheets to mTileSheetHandler */
    QDir assetsDir("../tme-2/assets");

    if(!assetsDir.exists()) {
        std::string debug = "Directory " + assetsDir.currentPath().toStdString() + " doesn't exist.";
        qDebug(debug.c_str());
    }

    QFileInfoList fileInfoList = assetsDir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);

    /* set up tilesheet tabs */
    std::for_each(fileInfoList.begin(), fileInfoList.end(), [this](const QFileInfo& fileInfo) {
        /* add tilesheet to our vector */
        int index = mTileSheetHandler.add(fileInfo.absoluteFilePath().toStdString(), QSize(32, 32));

        /* add a new tab with scroll area */
        //QWidget* tab = new QWidget(ui->tileSheetTabs);
        QWidget* tab = new QWidget(mTileSelector);
        tab->setObjectName("tileSheetTab");
        //QRect tabViewRect = ui->tileSheetTabs->geometry();
        QRect tabViewRect = mTileSelector->geometry();

        /* add tab's layout */
        QGridLayout* tabGridLayout = new QGridLayout(tab);
        tab->setLayout(tabGridLayout);
        tab->setGeometry(tabViewRect);

        /* create a scroll area for this tilesheet */
        QScrollArea* scrollArea = new QScrollArea(tab);
        QWidget* scrollAreaContents = new QWidget(scrollArea);
        scrollArea->setWidget(scrollAreaContents);

        /* add scroll area to tab's layout */
        tabGridLayout->addWidget(scrollArea);

        QRect tabRect = tab->geometry();
        scrollArea->setGeometry(tabRect);
        scrollAreaContents->setGeometry(tabRect);

        /* set scroll area's layout */
        setTileSelectLayout(scrollArea, scrollAreaContents, mTileSheetHandler.get(index));
        //ui->tileSheetTabs->addTab(tab, QString(std::to_string(index).c_str()));
        mTileSelector->addTab(tab, QString(std::to_string(index).c_str()));
    });
}

/*
 * creates a frame, scroll area, scroll area widget contents & its layout, and SFMLCanvas
 */
void MainWindow::createNewCanvasArea(int width, int height, int tileWidth, int tileHeight)
{
    if(nullptr != mCanvasFrame) delete mCanvasFrame;
    mCanvasFrame = new CanvasFrame(ui->centralwidget);

    resizeSFMLFrame(mCanvasFrame);

    if(0 == width || 0 == height) {
        width = getNearestMultiple(mCanvasFrame->size().width(), tileWidth);
        height = getNearestMultiple(mCanvasFrame->size().height(), tileHeight);
    }

    mCanvasFrame->init(QPoint(0, 0), QSize(width, height), mTileSheetHandler, tileWidth, tileHeight);
    mCanvasFrame->setStyleSheet("background-color:blue");

    /* add scroll area to main layout */
    mLayout->addWidget(mCanvasFrame, 0, 2, 2, 1);

    MyCanvas* canvas = mCanvasFrame->getCanvas();

    /* wire up buttons to canvas */
    auto tabs = this->findChildren<QWidget*> ("tileSheetTab");

    for(const auto& tab : tabs) {
        auto buttons = tab->findChildren<JPushButton*> ("tileSelectButton");

        for(const auto& button: buttons) {
            connect(button, SIGNAL(clicked(const sf::Rect<int>&, const std::shared_ptr<const TileSheet>)),
                canvas, SLOT(setCurrentTile(const sf::Rect<int>&, const std::shared_ptr<const TileSheet>)));
        }
    }

    /* reset tile sheet index */
    canvas->setCurrentTileSheetIndex(mTileSelector->getCurrentTabIndex());

    /* reconnect tab switching to change mTileSheet */
    connect(mTileSelector, SIGNAL(currentChanged(int)), canvas, SLOT(setCurrentTileSheetIndex(int)));

    /* reconnect canvas to tile information frame */
    connect(canvas, SIGNAL(clicked(const Tile&)), this, SLOT(sendTileInformation(const Tile&)));
}

void MainWindow::sendTileInformation(const Tile& tile)
{
    mCurrentTileFrame->receiveTileInformation(tile, mTileSheetHandler);
}

void MainWindow::sendTraversableInformation(const QString& str)
{
    bool isTraversable = false;

    if(!str.compare(QString("True"), Qt::CaseInsensitive)) {
        isTraversable = true;
    }

    mCanvasFrame->getCanvas()->setCurrentTileTraversable(isTraversable);
}

void MainWindow::setTileSelectLayout(QScrollArea* scrollArea, QWidget* scrollAreaContents, const std::shared_ptr<const TileSheet>& tileSheet)
{
    int tileSheetCols = tileSheet->getColumns();
    int tileSheetRows = tileSheet->getRows();

    int tileWidth = mCanvasFrame->getCanvas()->getTileMap().getTileWidth();
    int tileHeight = mCanvasFrame->getCanvas()->getTileMap().getTileHeight();

    /* layout for scrollarea */
    QGridLayout* layout = new QGridLayout(scrollAreaContents);
    scrollAreaContents->setLayout(layout);

    int gridCols = std::floor(scrollArea->geometry().width() / tileWidth);
    int gridRows = std::floor(scrollArea->geometry().height() / tileHeight);

    MyCanvas* canvas = mCanvasFrame->getCanvas();

    for(int i = 0; i < tileSheetRows; ++i) {
        for(int j = 0; j < tileSheetCols; ++j) {
            int xOffset = (j*tileWidth)+j;
            int yOffset = (i*tileHeight)+i;

            /* clip tilesheet to x, y, tilewidth, tileheight for tile icon*/
            QPixmap tile = tileSheet->getQtTileSheet().copy(xOffset, yOffset, tileWidth, tileHeight);

            /* create new push button */
            JPushButton* button = new JPushButton(this, tileSheet);
            button->setObjectName("tileSelectButton");

            //button->setClipBounds(yOffset, xOffset, mTileWidth, mTileHeight);
            button->setClipPosition(yOffset, xOffset);

            /* connect tile selector to canvas */
            connect(button, SIGNAL(clicked(const sf::Rect<int>&, const std::shared_ptr<const TileSheet>)),
                canvas, SLOT(setCurrentTile(const sf::Rect<int>&, const std::shared_ptr<const TileSheet>)));

            /* set button's icon */
            button->setIcon(QIcon(tile));
            button->setMaximumSize(tileWidth, tileHeight);

            int col = i%gridRows;
            int row = j%gridCols;
            layout->addWidget(button, row, col, Qt::AlignCenter);
        }
    }
}

void MainWindow::saveMap()
{
    /* open up a save file dialog */
    QString path = QFileDialog::getSaveFileName(this, "Save File", QCoreApplication::applicationDirPath());

    mCanvasFrame->getCanvas()->saveMap(path);
}

void MainWindow::showNewMapDialog()
{
    /* start new map dialog */
    NewMapDialog* nmd = new NewMapDialog(this);
    nmd->show();

    connect(nmd, SIGNAL(makeNew(std::tuple<int,int, int, int>)), this, SLOT(makeNewMap(std::tuple<int, int, int, int>)));
}

void MainWindow::makeNewMap(std::tuple<int, int, int, int> params)
{
    /*
     * width, height, tilewidth, tileheight: ints
     */
    auto width = std::get<0>(params);
    auto height = std::get<1>(params);
    auto tileWidth = std::get<2>(params);
    auto tileHeight = std::get<3>(params);

    createNewCanvasArea(width, height, tileWidth, tileHeight);
}

void MainWindow::setCurrentTileSheetIndex(int index)
{
    mTileSheetIndex = index;
}

void MainWindow::resizeCurrentTileFrame()
{
    mCurrentTileFrame->resize(mWindowWidth / 3, mWindowHeight / 3);
    mCurrentTileFrame->setMinimumHeight(mWindowHeight / 3);
}

void MainWindow::resizeSFMLFrame(QFrame* frame)
{
    int width = std::floor(mWindowWidth * (.66));
    frame->resize(width, mWindowHeight-100);
    frame->setMaximumSize(width, mWindowHeight-100);
}

MainWindow::~MainWindow()
{
    delete ui;
}
