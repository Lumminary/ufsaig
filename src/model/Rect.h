/*
 * Rect.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Sergii Kutnii
 */

#ifndef MODEL_RECT_H_
#define MODEL_RECT_H_

#include <cstddef>

namespace ImageServer {

struct Size {
    size_t width = 0;
    size_t height = 0;

    Size() = default;
    Size(size_t w, size_t h) : width{ w }, height{ h } {}
    Size(const Size& other) = default;

    Size aspectFit(const Size& box) const;
};

struct Point {
    size_t x = 0;
    size_t y = 0;

    Point() = default;
    Point(size_t xCoord, size_t yCoord) : x{ xCoord }, y{ yCoord } {}
    Point(const Point& other) = default;
};

struct Rect {
    Point origin;
    Size size;

    Rect() = default;
    Rect(size_t x, size_t y, size_t width, size_t height) :
        origin{ x, y }, size{ width, height } {}
    Rect(const Rect& other) = default;
};

}



#endif /* MODEL_RECT_H_ */
