/*
 * SpriteMaker.cpp
 *
 *  Created on: Feb 16, 2021
 *      Author: Sergii Kutnii
 */

#include "SpriteMaker.h"
#include <json/json.h>
#include "utils/md5.h"
#include "utils/convert.h"

namespace ImageServer {

namespace Core {

using namespace httpserver;

//------------------------------------------------------------------------------

const std::shared_ptr<httpserver::http_response> SpriteMaker::render(
        const httpserver::http_request &req) {
    try {
        const std::vector<char>& data = req.get_content();
        const char *dataStart = data.data();
        const char *dataEnd = dataStart + data.size();

        std::string key = "sp:"
                + Utils::md5String((const unsigned char*)dataStart,
                        data.size());

        auto args = req.get_args();
        std::string forceCreateStr = args["force_create"];

        bool forceCreate = !forceCreateStr.empty()
                && Utils::toBool(forceCreateStr);

        std::string cssPrefix = args["css_prefix"];

        if (!forceCreate
                && spriteController->imageStore->metaConnector->hasEntry(key))
            return makeResponse(key, cssPrefix);


        Json::Value descriptor;
        Json::CharReaderBuilder readerBuilder;
        std::string errs;

        std::unique_ptr<Json::CharReader>
        reader{ readerBuilder.newCharReader() };

        reader->parse(dataStart,
                dataEnd, &descriptor, &errs);

        std::string format = descriptor["output_format"].asString();
        Json::Value& tilesDescriptor = descriptor["original_images_size"];
        if (!tilesDescriptor.isArray())
            return std::make_shared<string_response>(
                    "No tiles", 400, "text/plain");

        unsigned int tileCount = tilesDescriptor.size();
        Sprite sprite;
        sprite.tiles.reserve(tileCount);
        sprite.extension = format;
        std::vector<ImageDescriptor> descriptors;
        descriptors.reserve(tileCount);

        for (unsigned int i = 0; i < tileCount; ++i) {
            Json::Value tileDesc = tilesDescriptor[i];
            std::string key = tileDesc["key"].asString();
            if (key.empty())
                continue;

            sprite.tiles.emplace_back();
            Tile& tile = sprite.tiles[sprite.tiles.size() - 1];
            tile.padding = sprite.tilePadding;

            tile.imageKey = key;
            tile.bounds.size.width = tileDesc["width"].asUInt();
            tile.bounds.size.height = tileDesc["height"].asUInt();

            descriptors.emplace_back();
            ImageDescriptor& desc = descriptors[descriptors.size() - 1];
            desc.imageKey = key;
            desc.save = tileDesc["save"].asBool();

            size_t scaledWidth =
                    tile.bounds.size.width * spriteController->dpiScaling;
            size_t scaledHeight =
                    tile.bounds.size.height * spriteController->dpiScaling;

            desc.width = scaledWidth;
            desc.height = scaledHeight;

            tile.originalSize.width = scaledWidth;
            tile.originalSize.height = scaledHeight;
        }

        DataMap tilesData = imageController->getImagesData(descriptors);

        for (Tile& tile: sprite.tiles)
            tile.data = tilesData[tile.imageKey];

        sprite.build();

        std::string background = descriptor["background"].asString();
        spriteController->saveSprite(key, sprite, background, tilesDescriptor);

        return makeResponse(key, cssPrefix);
    } catch(std::exception& e) {
        return std::make_shared<string_response>(
                e.what(), 400, "text/plain");
    }
}

//------------------------------------------------------------------------------

const std::shared_ptr<httpserver::http_response>
SpriteMaker::makeResponse(
        const std::string &spriteKey, const std::string &cssPrefix) {
    if (cssPrefix.empty())
        return std::make_shared<string_response>(spriteKey,
                        http::http_utils::http_ok,
                        "text/plain");

    std::string cssLink =
            cssEndpoint +
            "/" + spriteKey +
            "/" + cssPrefix;
    auto response =
            std::make_shared<string_response>("",
                    http::http_utils::http_see_other, "text/plain");
    response->with_header("Location", cssLink);
    return response;
}

}

} /* namespace ImageServer */
