#ifndef LINE_H
#define LINE_H

#include <SFML/Graphics.hpp>

/*
 * Basic class for SFML lines
 */
class Line
{
private:
    sf::Vertex mStart;
    sf::Vertex mEnd;

    void setLine();

public:
    Line();
    Line(const sf::Vertex& start, const sf::Vertex& end);
    Line(int startX, int startY, int endX, int endY);

    sf::Vertex mLine[2];

    void setPoints(int startX, int startY, int endX, int endY);
};

#endif // LINE_H
