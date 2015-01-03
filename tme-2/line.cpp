#include "line.h"

Line::Line()
{
}

Line::Line(const sf::Vertex& start, const sf::Vertex& end) :
    mStart(start), mEnd(end)
{
    setLine();
}

Line::Line(int startX, int startY, int endX, int endY) :
    mStart(sf::Vector2f(startX, startY)),
    mEnd(sf::Vector2f(endX, endY))
{
    setLine();
}

void Line::setLine()
{
    mLine[0] = mStart;
    mLine[1] = mEnd;
}

void Line::setPoints(int startX, int startY, int endX, int endY)
{
    mStart.position = sf::Vector2f(startX, startY);
    mEnd.position = sf::Vector2f(endX, endY);
    setLine();
}
