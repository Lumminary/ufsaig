/*
 * Sprite.cpp
 *
 *  Created on: Feb 15, 2021
 *      Author: Sergii Kutnii
 */

#include "Sprite.h"
#include <algorithm>
#include <unordered_set>

namespace ImageServer {

//------------------------------------------------------------------------------

struct SpriteNode;
using SpriteNodePtr = std::shared_ptr<SpriteNode>;

struct SpriteNode {
    Rect bounds;
    Tile *tile = nullptr;

    std::unordered_set<SpriteNodePtr> subnodes;

    bool free() const {
        return (tile == nullptr);
    }

    bool place(Tile* tile);

    SpriteNodePtr growToFit(Tile* tile);
    SpriteNodePtr growRight(size_t extraWidth);
    SpriteNodePtr growDown(size_t extraHeight);
};

//------------------------------------------------------------------------------

SpriteNodePtr SpriteNode::growRight(size_t extraWidth) {
    SpriteNodePtr right = std::make_shared<SpriteNode>();

    right->bounds = Rect{ bounds.origin.x + bounds.size.width,
        bounds.origin.y,
        extraWidth,
        bounds.size.height
    };

    subnodes.insert(right);
    bounds.size.width += extraWidth;

    return right;
}

//------------------------------------------------------------------------------

SpriteNodePtr SpriteNode::growDown(size_t extraHeight) {
    SpriteNodePtr bottom = std::make_shared<SpriteNode>();

    bottom->bounds = Rect{ bounds.origin.x,
        bounds.origin.y + bounds.size.height,
        bounds.size.width,
        extraHeight };

    subnodes.insert(bottom);
    bounds.size.height += extraHeight;

    return bottom;
}

//------------------------------------------------------------------------------

bool SpriteNode::place(Tile* imageTile) {
   Size actualSize = imageTile->grossSize();

   if (free()
           && (bounds.size.width >= actualSize.width)
           && (bounds.size.height >= actualSize.height)) {
       tile = imageTile;
       tile->bounds.origin.x = bounds.origin.x + imageTile->padding;
       tile->bounds.origin.y = bounds.origin.y + imageTile->padding;

       size_t bottomHeight = bounds.size.height - actualSize.height;
       if (bottomHeight > 0) {
           SpriteNodePtr bottom = std::make_shared<SpriteNode>();
           bottom->bounds = Rect{ bounds.origin.x,
               bounds.origin.y + actualSize.height,
               bounds.size.width,
               bottomHeight };
           subnodes.insert(bottom);
       }

       size_t rightWidth = bounds.size.width - actualSize.width;
       if (rightWidth > 0) {
           SpriteNodePtr right = std::make_shared<SpriteNode>();
           right->bounds = Rect{ bounds.origin.x + actualSize.width,
               bounds.origin.y,
               rightWidth,
               bounds.size.height
           };
           subnodes.insert(right);
       }

       return true;
   }

   for (SpriteNodePtr adj: subnodes)
       if (adj->place(imageTile))
           return true;

   return false;
}

//------------------------------------------------------------------------------

SpriteNodePtr SpriteNode::growToFit(Tile* tile) {
    Size grossSize = tile->grossSize();

    bool canGrowDown = (grossSize.width <= bounds.size.width );
    bool canGrowRight = (grossSize.height <= bounds.size.height);
    bool shouldGrowRight = canGrowRight
            && (bounds.size.height >=
                    (bounds.size.width + grossSize.width));
    bool shouldGrowDown  = canGrowDown
            && (bounds.size.width >=
                    (bounds.size.height + grossSize.height));

    if (shouldGrowRight) {
        return growRight(grossSize.width);
    }
    else if (shouldGrowDown) {
        return growDown(grossSize.height);
    }
    else if (canGrowRight) {
        return growRight(grossSize.width);
    }

    return growDown(grossSize.height);
}

//------------------------------------------------------------------------------

void Sprite::build() {
    if (tiles.empty())
        return;

    std::vector<Tile*> tmpTiles;
    tmpTiles.reserve(tiles.size());
    for (Tile& tile: tiles)
        tmpTiles.push_back(&tile);

    std::sort(tmpTiles.begin(), tmpTiles.end(),
            [](const Tile* a, const Tile* b) -> bool {
                size_t h1 = a->bounds.size.height;
                size_t h2 = b->bounds.size.height;
                if (h1 < h2)
                    return false;
                else if (h2 < h1)
                    return true;

                if (a->bounds.size.width < b->bounds.size.width)
                    return false;

                return true;
            });

    SpriteNodePtr root = std::make_shared<SpriteNode>();
    root->bounds.size = tiles[0].grossSize();

    for (Tile* tile: tmpTiles) {
        if (root->place(tile))
            continue;

        SpriteNodePtr extension = root->growToFit(tile);
        extension->place(tile);
    }

    bounds = root->bounds;
}

}
