#ifndef JPUSHBUTTON_H
#define JPUSHBUTTON_H

#include <QPushButton>
#include <SFML/Graphics.hpp>
#include <memory>

#include "tilesheet.h"

class JPushButton : public QPushButton
{
    Q_OBJECT
private:
    typedef std::shared_ptr<const TileSheet> TileSheetPtr;

    sf::Rect<int> mClipBounds;
    const TileSheetPtr mTileSheetHandle;

public:
    explicit JPushButton(QWidget* parent, const TileSheetPtr tileSheetHandle);

    void setClipBounds(int top, int left, int width, int height);
    void setClipSize(int width, int height);
    void setClipPosition(int top, int left);
    sf::Rect<int> getClipBounds();

signals:
    void clicked(const sf::Rect<int>& bounds, const std::shared_ptr<const TileSheet> tileSheet);

public slots:

private slots:
    void sendTileToDraw();
};

#endif // JPUSHBUTTON_H
