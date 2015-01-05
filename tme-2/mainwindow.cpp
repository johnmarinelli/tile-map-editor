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
    mSFMLView(nullptr),
    mTileSheetIndex(0),
    mTileWidth(DEFAULT_TILE_WIDTH),
    mTileHeight(DEFAULT_TILE_HEIGHT),
    mWindowWidth(0),
    mWindowHeight(0)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    mWindowWidth = QApplication::desktop()->availableGeometry().width();
    mWindowHeight = QApplication::desktop()->availableGeometry().height();
    this->setGeometry(0, 0, mWindowWidth, mWindowHeight);
    ui->centralwidget->setLayout(mLayout);

    resizeTileSelect();
    createNewCanvasArea();
    mCurrentTileFrame = new CurrentTileFrame(this);
    resizeCurrentTileFrame();

    setTileSheetTabs();

    mLayout->addWidget(mCurrentTileFrame, 0, 1, 1, 1);
    mLayout->addWidget(ui->tileSheetTabs, 1, 1, 1, 1);

    mLayout->setColumnStretch(1, 30);
    mLayout->setColumnStretch(2, 70);

    /* connect canvas to tile information frame */
    connect(mSFMLView, SIGNAL(clicked(const Tile&)), this, SLOT(sendTileInformation(const Tile&)));

    /* connect tab switching to change mTileSheet*/
    connect(ui->tileSheetTabs, SIGNAL(currentChanged(int)), mSFMLView, SLOT(setCurrentTileSheetIndex(int)));
    connect(ui->tileSheetTabs, SIGNAL(currentChanged(int)), this, SLOT(setCurrentTileSheetIndex(int)));

    /* connect new file option to new map dialog */
    connect(ui->newMapAction, SIGNAL(triggered()), this, SLOT(showNewMapDialog()));

    /* connect save map */
    connect(ui->saveMapAction, SIGNAL(triggered()), this, SLOT(saveMap()));
}

void MainWindow::setTileSheetTabs()
{
    /* add all tilesheets to mTileSheetHandler */
    QDir assetsDir("../tme-2/assets");
    QFileInfoList fileInfoList = assetsDir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);

    /* set up tilesheet tabs */
    std::for_each(fileInfoList.begin(), fileInfoList.end(), [this](const QFileInfo& fileInfo) {
        /* add tilesheet to our vector */
        int index = mTileSheetHandler.add(fileInfo.absoluteFilePath().toStdString(), QSize(32, 32));

        /* add a new tab with scroll area */
        QWidget* tab = new QWidget(ui->tileSheetTabs);
        tab->setObjectName("tileSheetTab");
        QRect tabViewRect = ui->tileSheetTabs->geometry();

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
        ui->tileSheetTabs->addTab(tab, QString(std::to_string(index).c_str()));
    });
}

/*
 * creates a frame, scroll area, scroll area widget contents & its layout, and SFMLCanvas
 */
void MainWindow::createNewCanvasArea(int width, int height, int tileWidth, int tileHeight)
{
    QFrame* sfmlFrame = new QFrame(ui->centralwidget);
    QScrollArea* sfmlScrollArea = new QScrollArea(sfmlFrame);
    QWidget* sfmlScrollAreaWidget = new QWidget(sfmlScrollArea);
    sfmlScrollArea->setWidget(sfmlScrollAreaWidget);

    sfmlFrame->setObjectName("SFMLFrame");
    sfmlScrollArea->setObjectName("SFMLScrollArea");
    sfmlScrollAreaWidget->setObjectName("SFMLScrollAreaWidget");
    resizeSFMLFrame(sfmlFrame);

    sfmlFrame->setStyleSheet("background-color:blue");

    /* add scroll area to main layout */
    mLayout->addWidget(sfmlFrame, 0, 2, 2, 1);
    std::cout << "Sfmlframe: " << sfmlFrame->geometry().width() << "\t" << sfmlFrame->geometry().height() << std::endl;

    if(nullptr == mSFMLView) {
        mSFMLView = new MyCanvas(sfmlScrollAreaWidget, QPoint(0,0),
                                 QSize(sfmlFrame->geometry().width(), sfmlFrame->geometry().height()),
                                 mTileSheetHandler,
                                 mTileWidth, mTileHeight);
    }
    else {
        width = getNearestMultiple(width, tileWidth);
        height = getNearestMultiple(height, tileHeight);

        QFrame* frame = this->findChild<QFrame*> ("SFMLFrame");
        delete mSFMLView;
        mSFMLView = nullptr;
        delete frame;
        frame = nullptr;

        sfmlFrame->show();
        sfmlScrollArea->show();
        sfmlScrollAreaWidget->show();

        sfmlScrollArea->resize(width, height);
        sfmlScrollAreaWidget->resize(width, height);

        mSFMLView = new MyCanvas(sfmlScrollAreaWidget, QPoint(0,0),
                                 QSize(width, height),
                                 mTileSheetHandler,
                                 tileWidth, tileHeight);

        //mSFMLView->show();
    }
    mSFMLView->setObjectName("canvas");

    /* add mSFMLView to scroll area */
    setCanvasScrollAreaLayout(sfmlScrollArea, sfmlScrollAreaWidget);
    resizeCanvasScrollArea(sfmlFrame, sfmlScrollArea, sfmlScrollAreaWidget);

    /* wire up buttons to canvas */
    auto tabs = this->findChildren<QWidget*> ("tileSheetTab");

    for(const auto& tab : tabs) {
        auto buttons = tab->findChildren<JPushButton*> ("tileSelectButton");

        for(const auto& button: buttons) {
            std::cout << button->getClipBounds().left << std::endl;
            connect(button, SIGNAL(clicked(const sf::Rect<int>&, const std::shared_ptr<const TileSheet>)),
                mSFMLView, SLOT(setCurrentTile(const sf::Rect<int>&, const std::shared_ptr<const TileSheet>)));
        }
    }

    /* reconnect tab switching to change mTileSheet */
    connect(ui->tileSheetTabs, SIGNAL(currentChanged(int)), mSFMLView, SLOT(setCurrentTileSheetIndex(int)));

    /* reconnect canvas to tile information frame */
    connect(mSFMLView, SIGNAL(clicked(const Tile&)), this, SLOT(sendTileInformation(const Tile&)));
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

    mSFMLView->setCurrentTileTraversable(isTraversable);
}

void MainWindow::setTileSelectLayout(QScrollArea* scrollArea, QWidget* scrollAreaContents, const std::shared_ptr<const TileSheet>& tileSheet)
{
    int tileSheetCols = tileSheet->getColumns();
    int tileSheetRows = tileSheet->getRows();

    /* layout for scrollarea */
    QGridLayout* layout = new QGridLayout(scrollAreaContents);
    scrollAreaContents->setLayout(layout);

    int gridCols = std::floor(scrollArea->geometry().width() / mTileWidth);
    int gridRows = std::floor(scrollArea->geometry().height() / mTileHeight);

    for(int i = 0; i < tileSheetRows; ++i) {
        for(int j = 0; j < tileSheetCols; ++j) {
            int xOffset = (j*mTileWidth)+j;
            int yOffset = (i*mTileHeight)+i;

            /* clip tilesheet to x, y, tilewidth, tileheight for tile icon*/
            QPixmap tile = tileSheet->getQtTileSheet().copy(xOffset, yOffset, mTileWidth, mTileHeight);

            /* create new push button */
            JPushButton* button = new JPushButton(this, tileSheet);
            button->setObjectName("tileSelectButton");

            //button->setClipBounds(yOffset, xOffset, mTileWidth, mTileHeight);
            button->setClipPosition(yOffset, xOffset);

            /* connect tile selector to canvas */
            connect(button, SIGNAL(clicked(const sf::Rect<int>&, const std::shared_ptr<const TileSheet>)),
                mSFMLView, SLOT(setCurrentTile(const sf::Rect<int>&, const std::shared_ptr<const TileSheet>)));

            /* set button's icon */
            button->setIcon(QIcon(tile));
            button->setMaximumSize(mTileWidth, mTileHeight);

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

    mSFMLView->saveMap(path);
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

void MainWindow::resizeTileSelect()
{
    ui->tileSheetTabs->resize(mWindowWidth / 3, mWindowHeight / 2);
}

void MainWindow::resizeSFMLFrame(QFrame* frame)
{
    int width = std::floor(mWindowWidth * (.66));
    frame->resize(width, mWindowHeight-100);
    frame->setMaximumSize(width, mWindowHeight-100);
}

/*
 * canvas scroll area design depends on mSFMLView
 */
void MainWindow::setCanvasScrollAreaLayout(QScrollArea* scrollArea, QWidget* scrollAreaWidgetContents)
{
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    removeLayout(scrollAreaWidgetContents->layout());

    QGridLayout* layout = new QGridLayout(scrollAreaWidgetContents);
    scrollAreaWidgetContents->setLayout(layout);
    scrollArea->setWidgetResizable(false);
    layout->addWidget(mSFMLView, 0, 0);
}

void MainWindow::resizeCanvasScrollArea(QFrame* frame, QScrollArea* scrollArea, QWidget* scrollAreaWidget)
{
    std::cout << "entering resize canvas scroll area" << std::endl;
    QRect SFMLFrameRect = frame->geometry();

    /* size of actual scroll area */
    //scrollArea->move(0, 0);
    scrollArea->resize(SFMLFrameRect.width(), SFMLFrameRect.height());
    scrollArea->setStyleSheet("background-color:red");

    std::cout << "Scroll area geometry: " << scrollArea->geometry().width() << "\t" << scrollArea->geometry().height() << std::endl;

    /* size of scroll area contents; determines scroll bars */
    scrollAreaWidget->resize(mSFMLView->size());
    scrollAreaWidget->setStyleSheet("background-color:orange");

    std::cout << "Scroll area widget geometry: " << scrollAreaWidget->geometry().width() << "\t" << scrollAreaWidget->geometry().height() << std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}
