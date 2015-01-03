#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <QLayout>
#include <QLayoutItem>

const int DEFAULT_TILE_WIDTH = 32;
const int DEFAULT_TILE_HEIGHT = 32;

/*
 * given actual T1, finds nearest multiple of T2.
 * usually used for snap-to-grid.
 */
template <typename T1, typename T2>
T1 getNearestMultiple(const T1& actual, const T2& multiple)
{
    return std::ceil(actual / multiple) * multiple;
}

/*
 * bool to string
 */
inline const char* boolToString(bool b)
{
    return b ? "True" : "False";
}

/*
 * delete a layout & its items
 */
void removeLayout(QLayout* layout);

#endif // UTILITY_H
