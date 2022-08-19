/*
 * SpriteController.cpp
 *
 *  Created on: Feb 20, 2021
 *      Author: Sergii Kutnii
 */

#include "SpriteController.h"
#include "utils/mime.h"
#include "utils/md5.h"
#include <sstream>
#include <unordered_set>
#include <algorithm>

namespace ImageServer {

using Dims = std::pair<std::string, std::string>;

}

namespace std {

template<> struct hash<ImageServer::Dims> {
public:

  size_t operator()(const ImageServer::Dims &dims) const {
      hash<string> strHash;
      return strHash(dims.first) ^ strHash(dims.second);
  }

};

}

namespace ImageServer {

using namespace Couchbase;

//------------------------------------------------------------------------------

namespace SpriteKeys {

const char *width = "width";
const char *height = "height";
const char *x = "x";
const char *y = "y";
const char *spriteSheetPosition = "sprite_sheet_position";
const char *imgW = "img_w";
const char *imgH = "img_h";
const char *bgSizeXPcnt = "bg_size_x_pcnt";
const char *bgSizeYPcnt = "bg_size_y_pcnt";
const char *bgPosXPcnt = "bg_pos_x_pcnt";
const char *bgPosYPcnt = "bg_pos_y_pcnt";
const char *cssArgs = "css_args";
const char *scale = "scale";
const char *image = "image";
const char *images = "images";
const char *spriteSheetImages = "sprite_sheet_images";
const char *spritesImages = "sprites_images";
const char *extension = "extension";
const char *mimeType = "mime_type";
const char *originalImagesSize = "original_images_size";
const char *spritesDPIScaling = "sprites_dpi_scaling";
const char *spritesPadding = "sprites_padding";
const char *type = "type";
const char *version = "version";
const char *createdTimestamp = "created_timestamp";
const char *key = "key";
const char *subType = "sub_type";

}

//------------------------------------------------------------------------------

static Json::Value
spriteSheetImages(const Sprite& sprite) {
    Json::Value entries;
    size_t maxScale = 3;

    for (const Tile& tile : sprite.tiles) {
        Json::Value entry;
        uint64_t width = tile.grossWidth() * maxScale;
        entry[SpriteKeys::width] = Json::UInt64{ width };
        uint64_t height = tile.grossHeight() * maxScale;
        entry[SpriteKeys::height] = Json::UInt64{ height };

        uint64_t offset = sprite.tilePadding * maxScale;
        Json::Value position;
        uint64_t x = (tile.bounds.origin.x - sprite.tilePadding) * maxScale;
        uint64_t y = (tile.bounds.origin.y - sprite.tilePadding) * maxScale;
        position[SpriteKeys::x] = Json::UInt64{ x };
        position[SpriteKeys::y] = Json::UInt64{ y };
        entry[SpriteKeys::spriteSheetPosition] = position;

        Json::Value cssArgs;
        cssArgs[SpriteKeys::imgW] = Json::UInt64{ tile.bounds.size.width };
        cssArgs[SpriteKeys::imgH] = Json::UInt64{ tile.bounds.size.height };
        double widthRatio = 100.0 * (double)sprite.bounds.size.width
                / (double)tile.bounds.size.width;
        double heightRatio = 100.0 * (double)sprite.bounds.size.height
                / (double)tile.bounds.size.height;
        cssArgs[SpriteKeys::bgSizeXPcnt] = widthRatio;
        cssArgs[SpriteKeys::bgSizeYPcnt] = heightRatio;

        double xRatio = -100.0 * (double)tile.bounds.origin.x
                / ((double)sprite.bounds.size.width
                        - (double)tile.bounds.size.width);
        double yRatio = -100.0 * (double)tile.bounds.origin.y
                / ((double)sprite.bounds.size.height
                        - (double)tile.bounds.size.height);
        cssArgs[SpriteKeys::bgPosXPcnt] = xRatio;
        cssArgs[SpriteKeys::bgPosYPcnt] = yRatio;

        entry[SpriteKeys::cssArgs] = cssArgs;

        entries[tile.imageKey] = entry;
    }

    return entries;
}

//------------------------------------------------------------------------------

std::string
SpriteController::spriteCSS(const std::string &key,
        const std::string& classPrefix) {
    EntryPtr metaEntry = imageStore->metaConnector->findEntry(key);
    if (!metaEntry)
        throw std::runtime_error("Metadata record not found");

    Json::Value meta = metaEntry->json();
    Json::Value& spriteSheetImages = meta[SpriteKeys::spriteSheetImages];
    std::vector<std::string> keys = spriteSheetImages.getMemberNames();

    std::stringstream styleStream;
    size_t imageCount = spriteSheetImages.size();

    std::unordered_set<Dims> dimensions;
    for (std::string &key: keys) {
        Json::Value& imageEntry = spriteSheetImages[key];
        Json::Value& css = imageEntry[SpriteKeys::cssArgs];

        styleStream << "div." << key << " "
                    << "{ " << std::endl
                    << "background-position: "
                    << css[SpriteKeys::bgPosXPcnt].asString() << "% "
                    << css[SpriteKeys::bgPosYPcnt].asString() << "%; "
                    << std::endl
                    << "background-size: "
                    << css[SpriteKeys::bgSizeXPcnt].asString() << "% "
                    << css[SpriteKeys::bgSizeYPcnt].asString() << "%; "
                    << std::endl
                    << "}" << std::endl;

        std::string width = css[SpriteKeys::imgW].asString();
        std::string height = css[SpriteKeys::imgH].asString();

        Dims dims{ width, height };
        if (dimensions.find(dims) != dimensions.end())
            continue;

        std::string boxClass = classPrefix + "w" + width + "h" + height;

        styleStream << "div." << boxClass << " {" << std::endl
                    << "width: " << width << "px;"
                    << std::endl
                    << "height: " << height << "px;"
                    << std::endl
                    << "display: block;" << std::endl
                    << "}" << std::endl;

        dimensions.insert(dims);
    }

    Json::Value& backgrounds = meta[SpriteKeys::spritesImages];
    if (!backgrounds.isArray() || (backgrounds.size() != 3))
        throw std::runtime_error("Invalid data format");

    std::vector<std::pair<std::string, double>> bgScales;
    for (int i = 0; i < 3; ++i) {
        Json::Value& entry = backgrounds[i];
        std::string bgKey = entry[SpriteKeys::key].asString();
        double scale = entry[SpriteKeys::scale].asDouble();
        bgScales.emplace_back(bgKey, scale);
    }

    std::sort(bgScales.begin(), bgScales.end(),
            [](auto& a, auto& b) {
       if (a.second < b.second)
           return true;

       return false;
    });

    std::string boxPrefix = classPrefix + "w";
    styleStream << "@media (-webkit-min-device-pixel-ratio: 1), "
                << " (min-resolution: 1.25dppx) { div[class^="
                << boxPrefix << "] { "
                << "background-image: "
                << "url(/sprite_sheet/"
                << key << "/"
                << bgScales[0].first
                << "); }}"
                << std::endl;

    styleStream << "@media (-webkit-min-device-pixel-ratio: 2), "
                << " (min-resolution: 2.25dppx) { div[class^="
                << boxPrefix << "] { "
                << "background-image: "
                << "url(/sprite_sheet/"
                << key << "/"
                << bgScales[1].first
                << "); }}"
                << std::endl;

    styleStream << "@media (-webkit-min-device-pixel-ratio: 3), "
                << " (min-resolution: 3.25dppx) { div[class^=sp2w] { "
                << "background-image: "
                << "url(/sprite_sheet/"
                << key << "/"
                << bgScales[2].first
                << "); }}"
                << std::endl;

    return styleStream.str();
}

//------------------------------------------------------------------------------

void
SpriteController::saveResolution(const std::string& spriteKey,
        const DataProviderPtr& spriteData,
        const Sprite& sprite, Json::Value& images,
        const std::string background,
        const std::pair<size_t, size_t>& scale) {

    size_t width = sprite.bounds.size.width * scale.first;
    size_t height = sprite.bounds.size.height * scale.first;
    std::string widthStr = std::to_string(width);
    std::string heightStr =  std::to_string(height);
    Json::Value meta;
    std::string resKey =
                    spriteKey
                    + ":" + widthStr
                    + "x" + heightStr
                    + ":" + sprite.extension;
    meta[SpriteKeys::key] = resKey;
    meta[SpriteKeys::scale] = (double)scale.first / (double)scale.second;

    imageStore->dataConnector->saveData(resKey,
            (const char *)spriteData->bytes(), spriteData->size());

    images.append(meta);
}

//------------------------------------------------------------------------------

void
SpriteController::saveSprite(const std::string& key,
        const Sprite& sprite,
        const std::string& background,
        const Json::Value& originals) {
    Json::Value metaData;
    metaData[SpriteKeys::spriteSheetImages] = spriteSheetImages(sprite);

    Json::Value resMeta;
    size_t scales[3] = { 1, 2, 3 };

    DataProviderPtr maxScaleSprite =
            renderer->renderSprite(sprite,
                    background, sprite.extension, 3);

    saveResolution(key,
            maxScaleSprite,
            sprite,
            resMeta,
            background,
            std::pair<size_t, size_t>{ 3, dpiScaling });

    Size maxSize{ 3 * sprite.bounds.size.width,
                        3 * sprite.bounds.size.height };

    Size doubleSize{ 2 * sprite.bounds.size.width,
                        2 * sprite.bounds.size.height };
    DataProviderPtr doubleScaleSprite =
            renderer->aspectFit(maxSize,
                    maxScaleSprite, doubleSize, sprite.extension);

    saveResolution(key,
            doubleScaleSprite,
            sprite,
            resMeta,
            background,
            std::pair<size_t, size_t>{ 2, dpiScaling });

    DataProviderPtr minScaleSprite =
            renderer->aspectFit(maxSize,
                    maxScaleSprite, sprite.bounds.size, sprite.extension);

    saveResolution(key,
            minScaleSprite,
            sprite,
            resMeta,
            background,
            std::pair<size_t, size_t>{ 1, dpiScaling });

    metaData[SpriteKeys::spritesImages] = resMeta;

    metaData[SpriteKeys::extension] = sprite.extension;
    metaData[SpriteKeys::mimeType] = Utils::mimeType(sprite.extension, true);
    metaData[SpriteKeys::originalImagesSize] = originals;
    metaData[SpriteKeys::spritesDPIScaling] = Json::UInt64{ dpiScaling };
    metaData[SpriteKeys::spritesPadding] =
            Json::UInt64{ dpiScaling * sprite.tilePadding };
    metaData[SpriteKeys::type] = "spritesheet-metadata";
    metaData[SpriteKeys::subType] = "sprites-metadata";

    metaData[SpriteKeys::version] = Json::UInt64{ 1 };
    metaData[SpriteKeys::createdTimestamp] =
            Json::UInt64{ (uint64_t)std::time(nullptr) };
    metaData[SpriteKeys::key] = key;

    Json::StreamWriterBuilder builder;
    std::string meta = Json::writeString(builder, metaData);

    imageStore->metaConnector->saveData(key, meta.c_str(), meta.size());
}

} /* namespace ImageServer */
