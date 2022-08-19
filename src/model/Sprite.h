/*
 * Sprite.h
 *
 *  Created on: Feb 15, 2021
 *      Author: Sergii Kutnii
 */

#ifndef MODEL_SPRITE_H_
#define MODEL_SPRITE_H_

#include "Tile.h"
#include <vector>

namespace ImageServer {

struct Sprite {
    Rect bounds;
    Tiles tiles;
    size_t tilePadding = 1;
    std::string extension;

    void build();
};

}

#endif /* MODEL_SPRITE_H_ */
