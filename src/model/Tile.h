/*
 * Tile.h
 *
 *  Created on: Feb 11, 2021
 *      Author: Sergii Kutnii
 */

#ifndef MODEL_TILE_H_
#define MODEL_TILE_H_

#include <string>
#include "DataProvider.h"
#include <memory>
#include <vector>
#include "Rect.h"

namespace ImageServer {

struct Tile {
    DataProviderPtr data;
    std::string imageKey;
    Rect bounds;
    Size originalSize;
    size_t padding;

    size_t grossWidth() const { return (bounds.size.width + 2 * padding); };
    size_t grossHeight() const { return (bounds.size.height + 2 * padding); };

    Size grossSize() const { return Size{ grossWidth(), grossHeight() }; };
};

using Tiles = std::vector<Tile>;

}

#endif /* MODEL_TILE_H_ */
