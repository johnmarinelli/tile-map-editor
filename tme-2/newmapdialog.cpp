#include "newmapdialog.h"
#include <QLabel>
#include <QIntValidator>
#include <tuple>
#include <iostream>

NewMapDialog::NewMapDialog(QWidget *parent) :
    QDialog(parent),
    mWidthInput(new QLineEdit(this)),
    mHeightInput(new QLineEdit(this)),
    mTileWidthInput(new QLineEdit(this)),
    mTileHeightInput(new QLineEdit(this)),
    mConfirm(new QPushButton("Create", this)),
    mLayout(new QVBoxLayout(this))
{
    this->setWindowModality(Qt::WindowModal);
    QIntValidator qiv(0, std::numeric_limits<int>::max());

    mWidthInput->setValidator(&qiv);
    mHeightInput->setValidator(&qiv);
    mTileWidthInput->setValidator(&qiv);
    mTileHeightInput->setValidator(&qiv);

    QLabel* widthLabel = new QLabel("Width: ", this);
    QLabel* heightLabel = new QLabel("Height: ", this);
    QLabel* tileWidthLabel = new QLabel("Tile Width: ", this);
    QLabel* tileHeightLabel = new QLabel("Tile Height: ", this);

    mLayout->addWidget(widthLabel, 0, Qt::AlignLeft);
    mLayout->addWidget(mWidthInput, 1, Qt::AlignRight);
    mLayout->addWidget(heightLabel, 0, Qt::AlignLeft);
    mLayout->addWidget(mHeightInput, 1, Qt::AlignRight);
    mLayout->addWidget(tileWidthLabel, 0, Qt::AlignLeft);
    mLayout->addWidget(mTileWidthInput, 1, Qt::AlignRight);
    mLayout->addWidget(tileHeightLabel, 0, Qt::AlignLeft);
    mLayout->addWidget(mTileHeightInput, 1, Qt::AlignRight);

    mLayout->addWidget(mConfirm, 1, Qt::AlignCenter);

    /* remap the clicked() to a custom slot */
    connect(mConfirm, SIGNAL(clicked()), this, SLOT(sendNewMapInfo()));
}
void NewMapDialog::sendNewMapInfo()
{
    try{
        mWidth = std::stoi(mWidthInput->text().toStdString());
        mHeight = std::stoi(mHeightInput->text().toStdString());
        mTileWidth = std::stoi(mTileWidthInput->text().toStdString());
        mTileHeight = std::stoi(mTileHeightInput->text().toStdString());

        auto package = std::make_tuple(mWidth, mHeight, mTileWidth, mTileHeight);
        emit makeNew(package);
    }
    catch(const std::invalid_argument& e) {
        /* if stoi is "" */
        std::cout << e.what() << std::endl;
    }
}
