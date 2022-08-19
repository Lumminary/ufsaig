/*
 * Rect.cpp
 *
 *  Created on: May 15, 2021
 *      Author: Sergii Kutnii
 */

#include "Rect.h"
#include <algorithm>

namespace ImageServer {

Size Size::aspectFit(const Size &box) const {
    double scale = std::min((double)box.width / (double)width,
            (double)box.height / (double)height);
    return Size{ (size_t)(width * scale), (size_t)(height * scale) };
}

}

