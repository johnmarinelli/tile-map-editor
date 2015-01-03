#ifndef NEWMAPDIALOG_H
#define NEWMAPDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

class NewMapDialog : public QDialog
{
    Q_OBJECT
private:
    QLineEdit* mWidthInput;
    QLineEdit* mHeightInput;
    QLineEdit* mTileWidthInput;
    QLineEdit* mTileHeightInput;

    int mWidth, mHeight, mTileWidth, mTileHeight;

    QPushButton* mConfirm;

    QVBoxLayout* mLayout;

public:
    explicit NewMapDialog(QWidget *parent = 0);

signals:
    void makeNew(std::tuple<int, int, int, int> package);

public slots:

private slots:
    void sendNewMapInfo();

};

#endif // NEWMAPDIALOG_H
