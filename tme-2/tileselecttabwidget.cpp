#include "tileselecttabwidget.h"
#include "jpushbutton.h"
#include <QGridLayout>
#include <QDir>
#include <QFileInfoList>

TileSelectTabWidget::TileSelectTabWidget(QWidget *parent) :
    QTabWidget(parent),
    mCurrentTabIndex(0)
{
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(setCurrentTabIndex(int)));
}

void TileSelectTabWidget::setTabLayout(QScrollArea *scrollArea, QWidget *scrollAreaContents, const std::shared_ptr<const TileSheet> &tileSheet, MyCanvas& canvas)
{
    int tileSheetCols = tileSheet->getColumns();
    int tileSheetRows = tileSheet->getRows();

    int tileWidth = canvas.getTileMap().getTileWidth();
    int tileHeight = canvas.getTileMap().getTileHeight();

    /* layout for scrollarea */
    QGridLayout* layout = new QGridLayout(scrollAreaContents);
    scrollAreaContents->setLayout(layout);

    int gridCols = std::floor(scrollArea->geometry().width() / tileWidth);
    int gridRows = std::floor(scrollArea->geometry().height() / tileHeight);

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
                &canvas, SLOT(setCurrentTile(const sf::Rect<int>&, const std::shared_ptr<const TileSheet>)));

            /* set button's icon */
            button->setIcon(QIcon(tile));
            button->setMaximumSize(tileWidth, tileHeight);

            int col = i%gridRows;
            int row = j%gridCols;
            layout->addWidget(button, row, col, Qt::AlignCenter);
        }
    }
}

void TileSelectTabWidget::setTabs(TileSheetHandler& tsh, MyCanvas& canvas)
{
        /* add all tilesheets to mTileSheetHandler */
        QDir assetsDir("../tme-2/assets");

        if(!assetsDir.exists()) {
            std::string debug = "Directory " + assetsDir.currentPath().toStdString() + " doesn't exist.";
            qDebug(debug.c_str());
        }

        QFileInfoList fileInfoList = assetsDir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);

        /* set up tilesheet tabs */
        std::for_each(fileInfoList.begin(), fileInfoList.end(), [&](const QFileInfo& fileInfo) {
            /* add tilesheet to our vector */
            int index = tsh.add(fileInfo.absoluteFilePath().toStdString(), QSize(32, 32));

            /* add a new tab with scroll area */
            //QWidget* tab = new QWidget(ui->tileSheetTabs);
            QWidget* tab = new QWidget(this);
            tab->setObjectName("tileSheetTab");
            //QRect tabViewRect = ui->tileSheetTabs->geometry();
            QRect tabViewRect = this->geometry();

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
            setTabLayout(scrollArea, scrollAreaContents, tsh.get(index), canvas);
            //ui->tileSheetTabs->addTab(tab, QString(std::to_string(index).c_str()));
            this->addTab(tab, QString(std::to_string(index).c_str()));
        });
}

void TileSelectTabWidget::setCurrentTabIndex(int index)
{
    mCurrentTabIndex = index;
}

int TileSelectTabWidget::getCurrentTabIndex() const
{
    return mCurrentTabIndex;
}
