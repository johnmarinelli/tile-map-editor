#ifndef JRADIOBUTTON_H
#define JRADIOBUTTON_H

#include <QRadioButton>
#include <SFML/Graphics.hpp>

class JRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit JRadioButton(const QString& text, sf::Texture& tilesheet, QWidget *parent = 0);

    void setIconPath(const QString path);
    void setClipBounds(int top, int left, int width, int height);
    sf::Rect<int> getClipBounds();

signals:

public slots:

private:
    QString mIconPath;
    sf::Texture& mTilesheet;

    sf::Rect<int> mClipBounds;
};

#endif // JRADIOBUTTON_H
