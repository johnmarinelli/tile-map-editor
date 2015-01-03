/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *newMapAction;
    QAction *saveMapAction;
    QWidget *centralwidget;
    QFrame *currentTileFrame;
    QLabel *currentTileGraphic;
    QComboBox *currentTileTraversable;
    QLabel *traversableLabel;
    QLabel *currentTileWorldCoords;
    QLabel *currentTileTileSheetCoords;
    QTabWidget *tileSheetTabs;
    QMenuBar *menubar;
    QMenu *menuTilemap_Editor_1;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        newMapAction = new QAction(MainWindow);
        newMapAction->setObjectName(QString::fromUtf8("newMapAction"));
        saveMapAction = new QAction(MainWindow);
        saveMapAction->setObjectName(QString::fromUtf8("saveMapAction"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        currentTileFrame = new QFrame(centralwidget);
        currentTileFrame->setObjectName(QString::fromUtf8("currentTileFrame"));
        currentTileFrame->setGeometry(QRect(0, 0, 171, 221));
        currentTileFrame->setFrameShape(QFrame::StyledPanel);
        currentTileFrame->setFrameShadow(QFrame::Raised);
        currentTileGraphic = new QLabel(currentTileFrame);
        currentTileGraphic->setObjectName(QString::fromUtf8("currentTileGraphic"));
        currentTileGraphic->setGeometry(QRect(10, 30, 158, 99));
        currentTileTraversable = new QComboBox(currentTileFrame);
        currentTileTraversable->setObjectName(QString::fromUtf8("currentTileTraversable"));
        currentTileTraversable->setGeometry(QRect(90, 170, 79, 23));
        traversableLabel = new QLabel(currentTileFrame);
        traversableLabel->setObjectName(QString::fromUtf8("traversableLabel"));
        traversableLabel->setGeometry(QRect(0, 170, 81, 16));
        currentTileWorldCoords = new QLabel(currentTileFrame);
        currentTileWorldCoords->setObjectName(QString::fromUtf8("currentTileWorldCoords"));
        currentTileWorldCoords->setGeometry(QRect(0, 110, 101, 16));
        currentTileTileSheetCoords = new QLabel(currentTileFrame);
        currentTileTileSheetCoords->setObjectName(QString::fromUtf8("currentTileTileSheetCoords"));
        currentTileTileSheetCoords->setGeometry(QRect(0, 140, 121, 16));
        tileSheetTabs = new QTabWidget(centralwidget);
        tileSheetTabs->setObjectName(QString::fromUtf8("tileSheetTabs"));
        tileSheetTabs->setGeometry(QRect(0, 220, 171, 281));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 20));
        menuTilemap_Editor_1 = new QMenu(menubar);
        menuTilemap_Editor_1->setObjectName(QString::fromUtf8("menuTilemap_Editor_1"));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuTilemap_Editor_1->menuAction());
        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(newMapAction);
        menuFile->addSeparator();
        menuFile->addAction(saveMapAction);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        newMapAction->setText(QApplication::translate("MainWindow", "New Map", 0, QApplication::UnicodeUTF8));
        newMapAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        saveMapAction->setText(QApplication::translate("MainWindow", "Save Map", 0, QApplication::UnicodeUTF8));
        saveMapAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        currentTileGraphic->setText(QString());
        currentTileTraversable->clear();
        currentTileTraversable->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "True", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "False", 0, QApplication::UnicodeUTF8)
        );
        traversableLabel->setText(QApplication::translate("MainWindow", "Traversable", 0, QApplication::UnicodeUTF8));
        currentTileWorldCoords->setText(QApplication::translate("MainWindow", "World Position:", 0, QApplication::UnicodeUTF8));
        currentTileTileSheetCoords->setText(QApplication::translate("MainWindow", "Tilesheet Position:", 0, QApplication::UnicodeUTF8));
        menuTilemap_Editor_1->setTitle(QApplication::translate("MainWindow", "Tilemap Editor 0.1", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
