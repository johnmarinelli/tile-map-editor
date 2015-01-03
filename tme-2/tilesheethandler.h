#ifndef TILESHEETHANDLER_H
#define TILESHEETHANDLER_H

#include <memory>
#include <map>
#include <vector>
#include "tilesheet.h"

/*
 * Noncopyable class to create, retrieve, and delete TileSheets
 */
class TileSheetHandler
{
private:
    typedef std::pair<unsigned short, std::shared_ptr<TileSheet>> TileSheetHandlerPair;

    std::map<unsigned short, std::shared_ptr<TileSheet>> mTileSheets;

    static unsigned short IDCounter;

public:
    TileSheetHandler();
    TileSheetHandler(const TileSheetHandler&) = delete;
    TileSheetHandler& operator=(const TileSheetHandler&) = delete;

    unsigned short add(const std::string& path, const QSize& tileSize);
    std::shared_ptr<TileSheet> get(unsigned short key) const;
    std::vector<std::shared_ptr<TileSheet>> getTileSheets() const;
    bool remove(unsigned short key);

};

#endif // TILESHEETHANDLER_H
