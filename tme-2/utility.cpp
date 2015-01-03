#include "utility.h"

void removeLayout(QLayout* layout)
{
    if(nullptr != layout) {
        QLayoutItem* item;
        while(nullptr != (item = layout->takeAt(0))) {
            layout->removeItem(item);
        }
        delete layout;
        layout = nullptr;
    }
}
