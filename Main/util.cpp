#include <iostream>
#include <algorithm>
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return (std::max(x1, x2) < std::min(x1 + w1, x2 + w2) && std::max(y1, y2) < std::min(y1 + h1, y2 + h2)); 
}

void velocityVector(int x1, int y1, int x2, int y2, float *dx, float *dy)
{
    // chuan hoa do dai cua vector van toc
    int length = std::max(abs(x1 - x2), abs(y1 - y2));

    if(length == 0)
    {
        *dx = *dy = 0;
        return;
    }

    // cap nhat dx, dy

    *dx = x1 - x2;
    *dx /= length;

    *dy = y1 - y2;
    *dy /= length;
}

